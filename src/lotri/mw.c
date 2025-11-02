#include "mw.h"


// GET & SET ===========================================================================================================
// CREATE & DELETE =====================================================================================================
static bool LOTRI_CreateMW_RK(LOTRI_MW* mw, const fastObjMesh* mesh, const char* file_mtl, const ModelSide side) {
    mw->model = LOTRI_CreateModel(mesh, file_mtl, side);
    mw->world = LOTRI_CreateWorld(mw->model);
    REQ_CONDITION(mw->model != NULL, return false);
    REQ_CONDITION(mw->world != NULL, return false);
    return true;
}
LOTRI_MW* LOTRI_CreateMW(const char* file_obj, const char *file_mtl, const ModelSide side) {

    LOTRI_MW* mw = calloc(1, sizeof(LOTRI_MW));
    REQ_CONDITION(mw != NULL, return NULL);

    fastObjMesh* mesh = fast_obj_read(file_obj);
    REQ_CONDITION(mesh != NULL, return NULL);
    REQ_CONDITION(LOTRI_CreateMW_RK(mw, mesh, file_mtl, side), {
        LOTRI_DeleteMW(mw);
        mw = NULL;
    });

    fast_obj_destroy(mesh);
    return mw;
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
bool LOTRI_DrawModelWorld(const LOTRI_Model* model, const LOTRI_World* world) {
    REQ_CONDITION(model != NULL, return false);
    REQ_CONDITION(world != NULL, return false);

    for (int i = 0; i < model->numFaces; i++) {
        if (world->faces[i].xyz.v.z > 0) continue;
        const Vec3i face = model->faces[i].ijk;

        SDL_RenderGeometryRaw(
            renderer, model->texture,
            (float*)&world->vertices[0].xyz, sizeof(LOTRI_Vertex),
            (SDL_FColor*)&world->vertices[0].rgba, sizeof(LOTRI_Vertex),
            (float*)&world->vertices[0].uv, sizeof(LOTRI_Vertex),
            model->numVertices, face.arr, 3, sizeof(int)
            );
    }
    return true;
}
bool LOTRI_DrawMW(const LOTRI_MW* mw) {
    if (mw == NULL) return false;
    const LOTRI_Model* model = mw->model;
    const LOTRI_World* world = mw->world;

    LOTRI_DrawModelWorld(model, world);
    return true;
}