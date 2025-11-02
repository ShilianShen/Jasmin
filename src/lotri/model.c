#include "model.h"

typedef struct LOTRI_Model LOTRI_Model;
typedef struct LOTRI_World LOTRI_World;

struct LOTRI_Model {
    int numVertices; LOTRI_Vertex *vertices;
    int numFaces; LOTRI_Face *faces;
    ModelSide side;
    SDL_Texture* texture;
};
struct LOTRI_World {
    Vec3f scale, position, rotation;

    int numVertices; LOTRI_Vertex *vertices;
    int numFaces; LOTRI_Face *faces;

    SDL_FRect* src;
    float depth;
};
struct LOTRI_MW {
    Vec3f scale, position, rotation;

    int numVertices;
    LOTRI_Vertex *modelVertices, *worldVertices;

    int numFaces;
    LOTRI_Face *modelFaces, *worldFaces;

    ModelSide side;
    SDL_Texture* texture;
    SDL_FRect* src;
    float depth;
};


// GET & SET ===========================================================================================================
bool LOTRI_GetModelPosition(const LOTRI_MW* mw, Vec3f* position) {
    REQ_CONDITION(mw != NULL, return false);
    REQ_CONDITION(position != NULL, return false);
    *position = mw->position;
    return true;
}
bool LOTRI_GetModelWorldVertex(const LOTRI_MW* mw, const int index, Vec3f* vec) {
    if (mw == NULL) return false;
    if (index >= mw->numVertices) return false;

    *vec = mw->worldVertices[index].xyz;
    return true;
}
bool LOTRI_GetModelModelVertex(const LOTRI_MW* mw, const int index, Vec3f* vec) {
    if (mw == NULL) return false;
    if (index >= mw->numVertices) return false;

    *vec = mw->modelVertices[index].xyz;
    return true;
}
bool LOTRI_GetModelDepth(const LOTRI_MW* mw, float* depth) {
    REQ_CONDITION(mw != NULL, return false);
    REQ_CONDITION(depth != NULL, return false);
    *depth = mw->depth;
    return true;
}
bool LOTRI_SetModelSrc(LOTRI_MW* mw, SDL_FRect* src) {
    if (mw == NULL) return false;

    mw->src = src;
    return true;
}
bool LOTRI_SetModelScale(LOTRI_MW* mw, const Vec3f scale) {
    REQ_CONDITION(mw != NULL, return false);
    mw->scale = scale;
    return true;
}
bool LOTRI_SetModelPosition(LOTRI_MW* mw, const Vec3f position) {
    REQ_CONDITION(mw != NULL, return false);
    mw->position = position;
    return true;
}
bool LOTRI_SetModelRotation(LOTRI_MW* mw, const Vec3f rotation) {
    REQ_CONDITION(mw != NULL, return false);
    mw->rotation = rotation;
    return true;
}
bool LOTRI_SetModelNormals(const LOTRI_MW* mw, const ModelSide side) {
    if (mw == NULL) return false;
    if (side == MODEL_SIDE_NULL) return true;

    for (int i = 0; i < mw->numFaces; i++) {
        const Vec3i index = mw->modelFaces[i].ijk;
        const Vec3f a = mw->modelVertices[index.v.i].xyz;
        const Vec3f b = mw->modelVertices[index.v.j].xyz;
        const Vec3f c = mw->modelVertices[index.v.k].xyz;
        const Vec3f normal = BASIC_GetNormal(a, b, c);
        const Vec3f sum = BASIC_GetSum(a, b, c);
        if (side == MODEL_SIDE_IN ^ BASIC_GetDot(normal, sum) < 0) {
            mw->modelFaces[i].xyz = (Vec3f){-normal.v.x, -normal.v.y, -normal.v.z};
        }
        else {
            mw->modelFaces[i].xyz = normal;
        }
    }
    return true;
}


// CREATE & DELETE =====================================================================================================
void LOTRI_DestroyModel(LOTRI_MW* mw) {
    if (mw != NULL) {
        if (mw->modelVertices != NULL) {
            free(mw->modelVertices);
            mw->modelVertices = NULL;
        }
        if (mw->worldVertices != NULL) {
            free(mw->worldVertices);
            mw->worldVertices = NULL;
        }
        if (mw->texture != NULL) {
            SDL_DestroyTexture(mw->texture);
            mw->texture = NULL;
        }
        if (mw->modelFaces != NULL) {
            free(mw->modelFaces);
            mw->modelFaces = NULL;
        }
        if (mw->worldFaces != NULL) {
            free(mw->worldFaces);
            mw->worldFaces = NULL;
        }
        free(mw);
    }
}
static bool LOTRI_CreateModel_RK(LOTRI_MW* mw, const fastObjMesh* mesh, const char* file_mtl) {
    memset(mw, 0, sizeof(LOTRI_MW));
    mw->numVertices = (int)mesh->position_count;
    mw->modelVertices = calloc(mw->numVertices, sizeof(LOTRI_Vertex));
    mw->worldVertices = calloc(mw->numVertices, sizeof(LOTRI_Vertex));
    if (mw->modelVertices == NULL || mw->worldVertices == NULL) {
        printf("%s: Failed to allocate memory for LOTRI_CreateModel_RK\n", __func__);
        return false;
        }
    for (int i = 0; i < mw->numVertices; i++) {
        mw->modelVertices[i].xyz = (Vec3f){mesh->positions[3*i], mesh->positions[3*i+1], mesh->positions[3*i+2]};
        mw->worldVertices[i].rgba = mw->modelVertices[i].rgba = (Vec4f){1, 1, 1, 1};
        mw->worldVertices[i].uv = mw->modelVertices[i].uv = (Vec2f){mesh->texcoords[2*i], 1-mesh->texcoords[2*i+1]};
    }

    mw->numFaces = (int)mesh->face_count;
    mw->modelFaces = calloc(mw->numFaces, sizeof(LOTRI_Face));
    mw->worldFaces = calloc(mw->numFaces, sizeof(LOTRI_Face));
    if (mw->worldFaces == NULL || mw->modelFaces == NULL) {
        printf("%s: Failed to allocate memory for LOTRI_CreateModel_RK\n", __func__);
        return false;
        }
    for (int i = 0; i < mw->numFaces; i++) {
        mw->modelFaces[i].ijk = (Vec3i){(int)mesh->indices[3*i].p, (int)mesh->indices[3*i+1].p, (int)mesh->indices[3*i+2].p,};
        mw->modelFaces[i].xyz = (Vec3f){mesh->normals[3*i+3], mesh->normals[3*i+4], mesh->normals[3*i+5],};
    }

    MTLMaterial materials[1];
    const int num_materials = parse_mtl_file(file_mtl, materials, 1);
    mw->texture = IMG_LoadTexture(renderer, materials[0].map_Kd);
    if (mw->texture == NULL) {
        printf("%s: Failed to load texture\n", __func__);
        return false;
    }
    SDL_SetTextureScaleMode(mw->texture, SDL_SCALEMODE_NEAREST);

    return true;
}
LOTRI_MW* LOTRI_CreateModel(const char* file_obj, const char *file_mtl, const ModelSide side) {

    LOTRI_MW* mw = malloc(sizeof(LOTRI_MW));
    if (mw == NULL) {
        printf("%s: Failed to \n", __func__);
        return false;
    }

    fastObjMesh* mesh = fast_obj_read(file_obj);
    if (mesh == NULL) {
        printf("%s: Failed to read OBJ file\n", __func__);
        return false;
    }

    if (LOTRI_CreateModel_RK(mw, mesh, file_mtl) == false) {
        printf("%s: Failed to create LOTRI modelVertices\n", __func__);
        LOTRI_DestroyModel(mw);
        mw = NULL;
    }
    else {
        mw->scale = (Vec3f){1, 1, 1};
        mw->side = side;
        LOTRI_SetModelNormals(mw, mw->side);
    }
    fast_obj_destroy(mesh);


    return mw;
}


// RENEW ===============================================================================================================
static void LOTRI_RenewModel_Depth(LOTRI_MW* mw) {
    float depth = 0;
    switch (mw->side) {
        case MODEL_SIDE_IN: {
            for (int i = 0; i < mw->numVertices; i++) {
                depth = SDL_max(depth, mw->worldVertices[i].xyz.v.z);
            }
            break;
        }
        case MODEL_SIDE_OUT: {
            for (int i = 0; i < mw->numVertices; i++) {
                depth = SDL_min(depth, mw->worldVertices[i].xyz.v.z);
            }
            break;
        }
        default: {
            for (int i = 0; i < mw->numVertices; i++) {
                depth += mw->worldVertices[i].xyz.v.z;
            }
            depth /= (float)mw->numVertices;
            break;
        }
    }
    mw->depth = depth;
}
bool LOTRI_RenewModel(LOTRI_MW* mw) {
    if (mw == NULL) return false;

    if (mw->side == MODEL_SIDE_CAMERA) {
        mw->rotation = (Vec3f){0, -camera.rotation.v.y, camera.rotation.v.z + (float)M_PI};
    }

    const Mat4f matArr[] = {
        BASIC_GetMatS(mw->scale),
        BASIC_GetMatR(mw->rotation),
        BASIC_GetMatT(mw->position),
        camera.mat,
    };
    const Mat4f mat = BASIC_GetProd(len_of(matArr), matArr);

    for (int i = 0; i < mw->numVertices; i++) {
        mw->worldVertices[i].xyz = BASIC_GetV3M4(mw->modelVertices[i].xyz, mat, true);
        mw->worldVertices[i].uv = BASIC_GetV2Rect(mw->modelVertices[i].uv, mw->src);
    }

    for (int i = 0; i < mw->numFaces; i++) {
        mw->worldFaces[i].ijk = mw->modelFaces[i].ijk;
        mw->worldFaces[i].xyz = BASIC_GetV3M4(mw->modelFaces[i].xyz, mat, false);
    }
    LOTRI_RenewModel_Depth(mw);

    return true;
}


// DRAW ================================================================================================================
bool LOTRI_DrawModel(const LOTRI_MW* mw) {
    if (mw == NULL) return false;

    for (int i = 0; i < mw->numFaces; i++) {
        if (mw->worldFaces[i].xyz.v.z > 0) continue;
        const Vec3i face = mw->modelFaces[i].ijk;

        SDL_RenderGeometryRaw(
            renderer, mw->texture,
            (float*)&mw->worldVertices[0].xyz, sizeof(LOTRI_Vertex),
            (SDL_FColor*)&mw->worldVertices[0].rgba, sizeof(LOTRI_Vertex),
            (float*)&mw->worldVertices[0].uv, sizeof(LOTRI_Vertex),
            mw->numVertices, face.arr, 3, sizeof(int)
            );
    }
    return true;
}