#include "model.h"



struct LOTRI_Model {
    int numVertices;
    LOTRI_Vertex *vertices;
    int numFaces;
    LOTRI_Face *faces;
    ModelSide side;
    SDL_Texture* texture;
};
struct LOTRI_World {
    Vec3f scale, position, rotation;
    int numVertices, numFaces;
    LOTRI_Vertex *vertices;
    LOTRI_Face *faces;
    SDL_FRect* src;
    float depth;
};



// GET & SET ===========================================================================================================
bool LOTRI_GetWorldPosition(const LOTRI_World *world, Vec3f* position) {
    REQ_CONDITION(world != NULL, return false);
    REQ_CONDITION(position != NULL, return false);
    *position = world->position;
    return true;
}
bool LOTRI_GetModelWorldVertex(const LOTRI_MW* mw, const int index, Vec3f* vec) {
    if (mw == NULL) return false;
    if (index >= mw->model->numVertices) return false;

    *vec = mw->world->vertices[index].xyz;
    return true;
}
bool LOTRI_GetModelModelVertex(const LOTRI_MW* mw, const int index, Vec3f* vec) {
    if (mw == NULL) return false;
    if (index >= mw->model->numVertices) return false;

    *vec = mw->model->vertices[index].xyz;
    return true;
}
bool LOTRI_GetModelDepth(const LOTRI_MW* mw, float* depth) {
    REQ_CONDITION(mw != NULL, return false);
    REQ_CONDITION(depth != NULL, return false);
    *depth = mw->world->depth;
    return true;
}
bool LOTRI_SetModelSrc(LOTRI_MW* mw, SDL_FRect* src) {
    if (mw == NULL) return false;

    mw->world->src = src;
    return true;
}
bool LOTRI_SetModelScale(LOTRI_MW* mw, const Vec3f scale) {
    REQ_CONDITION(mw != NULL, return false);
    mw->world->scale = scale;
    return true;
}
bool LOTRI_SetModelPosition(LOTRI_MW* mw, const Vec3f position) {
    REQ_CONDITION(mw != NULL, return false);
    mw->world->position = position;
    return true;
}
bool LOTRI_SetModelRotation(LOTRI_MW* mw, const Vec3f rotation) {
    REQ_CONDITION(mw != NULL, return false);
    mw->world->rotation = rotation;
    return true;
}
bool LOTRI_SetModelNormals(const LOTRI_MW* mw, const ModelSide side) {
    if (mw == NULL) return false;
    if (side == MODEL_SIDE_NULL) return true;

    for (int i = 0; i < mw->model->numFaces; i++) {
        const Vec3i index = mw->model->faces[i].ijk;
        const Vec3f a = mw->model->vertices[index.v.i].xyz;
        const Vec3f b = mw->model->vertices[index.v.j].xyz;
        const Vec3f c = mw->model->vertices[index.v.k].xyz;
        const Vec3f normal = BASIC_GetNormal(a, b, c);
        const Vec3f sum = BASIC_GetSum(a, b, c);
        if (side == MODEL_SIDE_IN ^ BASIC_GetDot(normal, sum) < 0) {
            mw->model->faces[i].xyz = (Vec3f){-normal.v.x, -normal.v.y, -normal.v.z};
        }
        else {
            mw->model->faces[i].xyz = normal;
        }
    }
    return true;
}


// CREATE & DELETE =====================================================================================================



static bool LOTRI_CreateModel(LOTRI_Model* model, const fastObjMesh* mesh, const char* file_mtl) {
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
    return true;
}
static bool LOTRI_CreateWorld_RK(LOTRI_World* world, const LOTRI_Model* model) {
    world->vertices = calloc(model->numVertices, sizeof(LOTRI_Vertex));
    REQ_CONDITION(world->vertices != NULL, return false);
    for (int i = 0; i < model->numVertices; i++) {
        world->vertices[i].rgba = model->vertices[i].rgba;
        world->vertices[i].uv = model->vertices[i].uv;
    }
    world->faces = calloc(model->numFaces, sizeof(LOTRI_Face));
    REQ_CONDITION(world->faces != NULL, return false);
    return true;
}
static bool LOTRI_CreateMW_RK(LOTRI_MW* mw, const fastObjMesh* mesh, const char* file_mtl) {
    REQ_CONDITION(LOTRI_CreateModel(mw->model, mesh, file_mtl), return false);
    REQ_CONDITION(LOTRI_CreateWorld_RK(mw->world, mw->model), return false);
    return true;
}

LOTRI_MW* LOTRI_CreateMW(const char* file_obj, const char *file_mtl, const ModelSide side) {

    LOTRI_MW* mw = malloc(sizeof(LOTRI_MW));
    REQ_CONDITION(mw != NULL, return NULL);
    memset(mw, 0, sizeof(LOTRI_MW));

    mw->model = calloc(1, sizeof(LOTRI_Model));
    mw->world = malloc(sizeof(LOTRI_World));

    fastObjMesh* mesh = fast_obj_read(file_obj);
    REQ_CONDITION(mesh != NULL, return NULL);
    
    if (LOTRI_CreateMW_RK(mw, mesh, file_mtl) == false) {
        printf("%s: Failed to create LOTRI modelVertices\n", __func__);
        LOTRI_DeleteMW(mw);
        mw = NULL;
    }
    else {
        mw->world->scale = (Vec3f){1, 1, 1};
        mw->model->side = side;
        LOTRI_SetModelNormals(mw, mw->model->side);
    }
    fast_obj_destroy(mesh);


    return mw;
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
LOTRI_World* LOTRI_DeleteWorld(LOTRI_World* world) {
    if (world == NULL) {
        return world;
    }
    if (world->vertices != NULL) {
        free(world->vertices);
        world->vertices = NULL;
    }
    if (world->faces != NULL) {
        free(world->faces);
        world->faces = NULL;
    }
    return world;
}
LOTRI_MW* LOTRI_DeleteMW(LOTRI_MW *mw) {
    if (mw == NULL) return mw;

    mw->model = LOTRI_DeleteModel(mw->model);
    mw->world = LOTRI_DeleteWorld(mw->world);
    free(mw); mw = NULL;

    return NULL;
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