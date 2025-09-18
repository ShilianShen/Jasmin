#include "camera.h"


static Mat4f matProj;


Camera camera = {
    .position = {0, 0, 1.8f},
    .rotation = {0, 0, -(float)M_PI},
    .scale = {200, 200, 200},
};


// SET & GET ===========================================================================================================
bool LOTRI_SetCamera(const Vec3f rotation, const Vec3f position, const float time) {
    if (camera.rotation2.v.x == rotation.v.x &&
        camera.rotation2.v.y == rotation.v.y &&
        camera.rotation2.v.z == rotation.v.z
        ) {return true;}

    camera.position1 = camera.position;
    camera.rotation1 = camera.rotation;
    camera.t1 = camera.t;

    camera.position2 = position;
    camera.rotation2 = rotation;
    camera.t2 = camera.t1 + time;

    camera.rotation2.v.y = clip(-(float)M_PI_2, camera.rotation2.v.y, M_PI_2);
    camera.rotation2.v.z = loop(0, camera.rotation2.v.z, M_PI * 2);
    return true;
}


// RENEW ===============================================================================================================
static void LOTRI_RenewCamera_Proj() {
    matProj = (Mat4f){
        .m = {
            {0, 0, 1, 0},
            {-1, 0, 0, 0},
            {0, -1, 0, 0},
            {(float)windowWidth / 2, (float)windowHeight / 2, 0, 1},
        }
    };
}
bool LOTRI_RenewCamera() {
    LOTRI_RenewCamera_Proj();
    camera.t = (float)SDL_GetTicks();
    const float rate = (camera.t - camera.t1) / (camera.t2 - camera.t1);

    camera.position = LOTRI_AtvVec(camera.position1, camera.position2, rate, BASIC_AtvRank2);
    camera.rotation = LOTRI_AtvVec(camera.rotation1, camera.rotation2, rate, BASIC_AtvRank2);

    const Mat4f matArr[] = {
        LOTRI_GetInvT(camera.position),
        LOTRI_GetInvR(camera.rotation),
        LOTRI_GetMatS(camera.scale),
        matProj,
    };
    camera.mat = LOTRI_GetProd(len_of(matArr), matArr);

    DEBUG_SendMessageL("%s:\n", __func__);
    DEBUG_SendMessageL("%.2f\n", camera.t1);
    DEBUG_SendMessageL("%.2f\n", camera.t);
    DEBUG_SendMessageL("%.2f\n", camera.t2);

    DEBUG_SendMessageL("position\n");
    DEBUG_SendMessageL("[%.2f, %.2f, %.2f]\n", camera.position1.v.x, camera.position1.v.y, camera.position1.v.z);
    DEBUG_SendMessageL("[%.2f, %.2f, %.2f]\n", camera.position.v.x, camera.position.v.y, camera.position.v.z);
    DEBUG_SendMessageL("[%.2f, %.2f, %.2f]\n", camera.position2.v.x, camera.position2.v.y, camera.position2.v.z);

    DEBUG_SendMessageL("rotation\n");
    DEBUG_SendMessageL("[%.2f, %.2f, %.2f]\n", camera.rotation1.v.x, camera.rotation1.v.y, camera.rotation1.v.z);
    DEBUG_SendMessageL("[%.2f, %.2f, %.2f]\n", camera.rotation.v.x, camera.rotation.v.y, camera.rotation.v.z);
    DEBUG_SendMessageL("[%.2f, %.2f, %.2f]\n", camera.rotation2.v.x, camera.rotation2.v.y, camera.rotation2.v.z);
    return true;
}


// DRAW ================================================================================================================
