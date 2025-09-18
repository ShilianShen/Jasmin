#include "camera.h"


static Mat4f matProj;


Camera camera = {
    .position = {0, 0, 1.8f},
    .rotation = {0, 0, -(float)M_PI},
    .scale = {200, 200, 200},
};


// SET & GET ===========================================================================================================
bool LOTRI_SetCameraRotation(const Vec3f rotation) {
    camera.rotation = rotation;
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
static void LOTRI_RenewCamera_Rotation() {
    const float angle = 0.03f;
    if (DEVICE_GetKeyPressed(SDL_SCANCODE_LEFT)) camera.rotation.v.z += angle;
    if (DEVICE_GetKeyPressed(SDL_SCANCODE_RIGHT)) camera.rotation.v.z -= angle;
    if (DEVICE_GetKeyPressed(SDL_SCANCODE_DOWN)) camera.rotation.v.y += angle;
    if (DEVICE_GetKeyPressed(SDL_SCANCODE_UP)) camera.rotation.v.y -= angle;

    if (DEVICE_GetKeyPress(SDL_SCANCODE_Q)) camera.rotation.v.z += M_PI_2;
    if (DEVICE_GetKeyPress(SDL_SCANCODE_E)) camera.rotation.v.z -= M_PI_2;

    camera.rotation.v.y = clip(-(float)M_PI_2, camera.rotation.v.y, M_PI_2);
    camera.rotation.v.z = loop(0, camera.rotation.v.z, M_PI * 2);
}
static void LOTRI_RenewCamera_Position() {
    const float step = 0.01f;
    if (DEVICE_GetKeyPressed(SDL_SCANCODE_W)) {
        camera.position.v.x += step * SDL_cosf(camera.rotation.v.z);
        camera.position.v.y += step * SDL_sinf(camera.rotation.v.z);
    }
    if (DEVICE_GetKeyPressed(SDL_SCANCODE_S)) {
        camera.position.v.x -= step * SDL_cosf(camera.rotation.v.z);
        camera.position.v.y -= step * SDL_sinf(camera.rotation.v.z);
    }
    if (DEVICE_GetKeyPressed(SDL_SCANCODE_A)) {
        camera.position.v.x -= step * SDL_sinf(camera.rotation.v.z);
        camera.position.v.y += step * SDL_cosf(camera.rotation.v.z);
    }
    if (DEVICE_GetKeyPressed(SDL_SCANCODE_D)) {
        camera.position.v.x += step * SDL_sinf(camera.rotation.v.z);
        camera.position.v.y -= step * SDL_cosf(camera.rotation.v.z);
    }
}
bool LOTRI_RenewCamera() {
    LOTRI_RenewCamera_Proj();
    LOTRI_RenewCamera_Rotation();
    LOTRI_RenewCamera_Position();
    Mat4f matArr[] = {
        LOTRI_GetInvT(camera.position),
        LOTRI_GetInvR(camera.rotation),
        LOTRI_GetMatS(camera.scale),
        matProj,
    };
    camera.mat = LOTRI_GetProd(len_of(matArr), matArr);

    DEBUG_SendMessageL("%s:\n", __func__);
    DEBUG_SendMessageL("    position: %.2f, %.2f, %.2f\n", camera.position.v.x, camera.position.v.y, camera.position.v.z);
    DEBUG_SendMessageL("    rotation: %.2f, %.2f, %.2f\n", camera.rotation.v.x, camera.rotation.v.y, camera.rotation.v.z);

    return true;
}


// DRAW ================================================================================================================
