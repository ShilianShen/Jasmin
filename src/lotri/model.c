#include "model.h"

typedef struct LOTRI_Model LOTRI_Model;
typedef struct LOTRI_World LOTRI_World;

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
struct LOTRI_MW {
    LOTRI_Model* model;
    LOTRI_World* world;
};


// GET & SET ===========================================================================================================
bool LOTRI_GetModelPosition(const LOTRI_MW* mw, Vec3f* position) {
    REQ_CONDITION(mw != NULL, return false);
    REQ_CONDITION(position != NULL, return false);
    *position = mw->world->position;
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
static bool LOTRI_CreateModel_RK(LOTRI_MW* mw, const fastObjMesh* mesh, const char* file_mtl) {

    mw->model->numVertices = (int)mesh->position_count;
    mw->model->vertices = calloc(mw->model->numVertices, sizeof(LOTRI_Vertex));
    mw->world->vertices = calloc(mw->model->numVertices, sizeof(LOTRI_Vertex));
    REQ_CONDITION(mw->model->vertices != NULL && mw->world->vertices != NULL, return false);

    for (int i = 0; i < mw->model->numVertices; i++) {
        mw->model->vertices[i].xyz = (Vec3f){mesh->positions[3*i], mesh->positions[3*i+1], mesh->positions[3*i+2]};
        mw->world->vertices[i].rgba = mw->model->vertices[i].rgba = (Vec4f){1, 1, 1, 1};
        mw->world->vertices[i].uv = mw->model->vertices[i].uv = (Vec2f){mesh->texcoords[2*i], 1-mesh->texcoords[2*i+1]};
    }

    mw->model->numFaces = (int)mesh->face_count;
    mw->model->faces = calloc(mw->model->numFaces, sizeof(LOTRI_Face));
    mw->world->faces = calloc(mw->model->numFaces, sizeof(LOTRI_Face));
    if (mw->world->faces == NULL || mw->model->faces == NULL) {
        printf("%s: Failed to allocate memory for LOTRI_CreateModel_RK\n", __func__);
        return false;
        }
    for (int i = 0; i < mw->model->numFaces; i++) {
        mw->model->faces[i].ijk = (Vec3i){(int)mesh->indices[3*i].p, (int)mesh->indices[3*i+1].p, (int)mesh->indices[3*i+2].p,};
        mw->model->faces[i].xyz = (Vec3f){mesh->normals[3*i+3], mesh->normals[3*i+4], mesh->normals[3*i+5],};
    }

    MTLMaterial materials[1];
    const int num_materials = parse_mtl_file(file_mtl, materials, 1);
    mw->model->texture = IMG_LoadTexture(renderer, materials[0].map_Kd);
    if (mw->model->texture == NULL) {
        printf("%s: Failed to load texture\n", __func__);
        return false;
    }
    SDL_SetTextureScaleMode(mw->model->texture, SDL_SCALEMODE_NEAREST);

    return true;
}
LOTRI_MW* LOTRI_CreateModel(const char* file_obj, const char *file_mtl, const ModelSide side) {

    LOTRI_MW* mw = malloc(sizeof(LOTRI_MW));
    REQ_CONDITION(mw != NULL, return NULL);
    memset(mw, 0, sizeof(LOTRI_MW));

    mw->model = calloc(1, sizeof(LOTRI_Model));
    mw->world = malloc(sizeof(LOTRI_World));

    fastObjMesh* mesh = fast_obj_read(file_obj);
    REQ_CONDITION(mesh != NULL, return NULL);
    
    if (LOTRI_CreateModel_RK(mw, mesh, file_mtl) == false) {
        printf("%s: Failed to create LOTRI modelVertices\n", __func__);
        LOTRI_DestroyModel(mw);
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
void LOTRI_DestroyModel(LOTRI_MW* mw) {
    if (mw != NULL) {
        if (mw->model->vertices != NULL) {
            free(mw->model->vertices);
            mw->model->vertices = NULL;
        }
        if (mw->world->vertices != NULL) {
            free(mw->world->vertices);
            mw->world->vertices = NULL;
        }
        if (mw->model->texture != NULL) {
            SDL_DestroyTexture(mw->model->texture);
            mw->model->texture = NULL;
        }
        if (mw->model->faces != NULL) {
            free(mw->model->faces);
            mw->model->faces = NULL;
        }
        if (mw->world->faces != NULL) {
            free(mw->world->faces);
            mw->world->faces = NULL;
        }
        free(mw);
    }
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