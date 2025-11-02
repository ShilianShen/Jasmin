#include "model.h"


// GET & SET ===========================================================================================================
bool LOTRI_GetModelVertex(const LOTRI_Model *model, const int index, Vec3f* vec) {
    if (model == NULL) return false;
    if (index >= model->numVertices) return false;
    *vec = model->vertices[index].xyz;
    return true;
}
bool LOTRI_SetModelNormals(const LOTRI_Model* model, const ModelSide side) {
    if (model == NULL) return false;
    if (side == MODEL_SIDE_NULL) return true;

    for (int i = 0; i < model->numFaces; i++) {
        const Vec3i index = model->faces[i].ijk;
        const Vec3f a = model->vertices[index.v.i].xyz;
        const Vec3f b = model->vertices[index.v.j].xyz;
        const Vec3f c = model->vertices[index.v.k].xyz;
        const Vec3f normal = BASIC_GetNormal(a, b, c);
        const Vec3f sum = BASIC_GetSum(a, b, c);
        if (side == MODEL_SIDE_IN ^ BASIC_GetDot(normal, sum) < 0) {
            model->faces[i].xyz = (Vec3f){-normal.v.x, -normal.v.y, -normal.v.z};
        }
        else {
            model->faces[i].xyz = normal;
        }
    }
    return true;
}


// CREATE & DELETE =====================================================================================================

static bool LOTRI_CreateModel_RK(LOTRI_Model* model, const fastObjMesh* mesh, const char* file_mtl, const ModelSide side) {
    model->numVertices = (int)mesh->position_count;
    model->vertices = calloc(model->numVertices, sizeof(LOTRI_Vertex));

    REQ_CONDITION(model->vertices != NULL, return false);

    for (int i = 0; i < model->numVertices; i++) {
        model->vertices[i].xyz = (Vec3f){mesh->positions[3*i], mesh->positions[3*i+1], mesh->positions[3*i+2]};
        model->vertices[i].rgba = (Vec4f){1, 1, 1, 1};
        model->vertices[i].uv = (Vec2f){mesh->texcoords[2*i], 1-mesh->texcoords[2*i+1]};
    }
    model->numFaces = (int)mesh->face_count;
    model->faces = calloc(model->numFaces, sizeof(LOTRI_Face));
    REQ_CONDITION(model->faces != NULL, return false);

    for (int i = 0; i < model->numFaces; i++) {
        model->faces[i].ijk = (Vec3i){(int)mesh->indices[3*i].p, (int)mesh->indices[3*i+1].p, (int)mesh->indices[3*i+2].p,};
        model->faces[i].xyz = (Vec3f){mesh->normals[3*i+3], mesh->normals[3*i+4], mesh->normals[3*i+5],};
    }

    MTLMaterial materials[1];
    const int num_materials = parse_mtl_file(file_mtl, materials, 1);
    model->texture = IMG_LoadTexture(renderer, materials[0].map_Kd);
    REQ_CONDITION(model->texture != NULL, return false);

    SDL_SetTextureScaleMode(model->texture, SDL_SCALEMODE_NEAREST);
    model->side = side;
    LOTRI_SetModelNormals(model, side);

    return true;
}
LOTRI_Model* LOTRI_CreateModel(const fastObjMesh* mesh, const char* file_mtl, const ModelSide side) {
    LOTRI_Model* model = calloc(1, sizeof(LOTRI_Model));
    REQ_CONDITION(model != NULL, return NULL);

    const bool RK = LOTRI_CreateModel_RK(model, mesh, file_mtl, side);
    REQ_CONDITION(RK, {
        model = LOTRI_DeleteModel(model);
        return NULL;
    });

    return model;
}


static bool LOTRI_CreateWorld_RK(LOTRI_World* world, const LOTRI_Model* model) {
    world->numVertices = model->numVertices;
    world->numFaces = model->numFaces;
    world->vertices = calloc(model->numVertices, sizeof(LOTRI_Vertex));
    REQ_CONDITION(world->vertices != NULL, return false);
    for (int i = 0; i < model->numVertices; i++) {
        world->vertices[i].rgba = model->vertices[i].rgba;
        world->vertices[i].uv = model->vertices[i].uv;
    }
    world->faces = calloc(model->numFaces, sizeof(LOTRI_Face));
    REQ_CONDITION(world->faces != NULL, return false);
    world->scale = (Vec3f){1, 1, 1};
    return true;
}
LOTRI_World* LOTRI_CreateWorld(const LOTRI_Model* model) {
    LOTRI_World* world = calloc(1, sizeof(LOTRI_World));
    REQ_CONDITION(world != NULL, return NULL);

    const bool RK = LOTRI_CreateWorld_RK(world, model);
    REQ_CONDITION(RK, {
        // world = LOTRI_DeleteWorld(world);
        return NULL;
    });

    return world;
}





LOTRI_Model* LOTRI_DeleteModel(LOTRI_Model* model) {
    if (model == NULL) {
        return model;
    }
    if (model->vertices != NULL) {
        free(model->vertices);
        model->vertices = NULL;
    }
    if (model->texture != NULL) {
        SDL_DestroyTexture(model->texture);
        model->texture = NULL;
    }
    if (model->faces != NULL) {
        free(model->faces);
        model->faces = NULL;
    }
    free(model);
    model = NULL;
    return model;
}



// RENEW ===============================================================================================================
static void LOTRI_RenewModel_Depth(LOTRI_MW* mw) {
    float depth = 0;
    switch (mw->model->side) {
        case MODEL_SIDE_IN: {
            for (int i = 0; i < mw->model->numVertices; i++) {
                depth = SDL_max(depth, mw->world->vertices[i].xyz.v.z);
            }
            break;
        }
        case MODEL_SIDE_OUT: {
            for (int i = 0; i < mw->model->numVertices; i++) {
                depth = SDL_min(depth, mw->world->vertices[i].xyz.v.z);
            }
            break;
        }
        default: {
            for (int i = 0; i < mw->model->numVertices; i++) {
                depth += mw->world->vertices[i].xyz.v.z;
            }
            depth /= (float)mw->model->numVertices;
            break;
        }
    }
    mw->world->depth = depth;
}
bool LOTRI_RenewModel(LOTRI_MW* mw) {
    if (mw == NULL) return false;

    if (mw->model->side == MODEL_SIDE_CAMERA) {
        mw->world->rotation = (Vec3f){0, -camera.rotation.v.y, camera.rotation.v.z + (float)M_PI};
    }

    const Mat4f matArr[] = {
        BASIC_GetMatS(mw->world->scale),
        BASIC_GetMatR(mw->world->rotation),
        BASIC_GetMatT(mw->world->position),
        camera.mat,
    };
    const Mat4f mat = BASIC_GetProd(len_of(matArr), matArr);

    for (int i = 0; i < mw->model->numVertices; i++) {
        mw->world->vertices[i].xyz = BASIC_GetV3M4(mw->model->vertices[i].xyz, mat, true);
        mw->world->vertices[i].uv = BASIC_GetV2Rect(mw->model->vertices[i].uv, mw->world->src);
    }

    for (int i = 0; i < mw->model->numFaces; i++) {
        mw->world->faces[i].ijk = mw->model->faces[i].ijk;
        mw->world->faces[i].xyz = BASIC_GetV3M4(mw->model->faces[i].xyz, mat, false);
    }
    LOTRI_RenewModel_Depth(mw);

    return true;
}


// DRAW ================================================================================================================
bool LOTRI_DrawModel(const LOTRI_MW* mw) {
    if (mw == NULL) return false;

    for (int i = 0; i < mw->model->numFaces; i++) {
        if (mw->world->faces[i].xyz.v.z > 0) continue;
        const Vec3i face = mw->model->faces[i].ijk;

        SDL_RenderGeometryRaw(
            renderer, mw->model->texture,
            (float*)&mw->world->vertices[0].xyz, sizeof(LOTRI_Vertex),
            (SDL_FColor*)&mw->world->vertices[0].rgba, sizeof(LOTRI_Vertex),
            (float*)&mw->world->vertices[0].uv, sizeof(LOTRI_Vertex),
            mw->model->numVertices, face.arr, 3, sizeof(int)
            );
    }
    return true;
}