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
// DRAW ================================================================================================================
