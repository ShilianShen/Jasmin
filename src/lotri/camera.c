#include "camera.h"


static Mat4f matProj;


Camera camera = {
    .position = {0, 0, 1.8f},
    .rotation = {0, 0, -(float)M_PI},
    .scale = {200, 200, 200},
};


// SET & GET ===========================================================================================================
bool LOTRI_SetCamera(const Vec3f rotation, const Vec3f position) {
    camera.rotation = rotation;
    camera.position = position;
    return true;
}


// RENEW ===============================================================================================================
static void LOTRI_RenewCamera_Proj() {
    matProj = (Mat4f){
        .m = {
            {0, 0, -1, 0},
            {1, 0, 0, 0},
            {0, -1, 0, 0},
            {windowRect.w / 2, windowRect.h / 2, 0, 1},
        }
    };
}
bool LOTRI_RenewCamera() {
    LOTRI_RenewCamera_Proj();

    const Mat4f matArr[] = {
        BASIC_GetInvT(camera.position),
        BASIC_GetInvR(camera.rotation),
        BASIC_GetMatS(camera.scale),
        matProj,
    };
    camera.mat = BASIC_GetProd(len_of(matArr), matArr);
    return true;
}


// DRAW ================================================================================================================
bool LOTRI_DrawCamera() {
    DEBUG_SendMessageL("%s:\n", __func__);
    DEBUG_SendMessageL("    position: %s\n", BASIC_GetStrVec3f(camera.position));
    DEBUG_SendMessageL("    rotation: %s\n", BASIC_GetStrVec3f(camera.rotation));
    return true;
}