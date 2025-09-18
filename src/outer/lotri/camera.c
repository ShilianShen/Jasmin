#include "camera.h"



const Mat4f matProj = {
    .m = {
        {0, 0, 1, 1},
        {-1, 0, 0, 0},
        {0, -1, 0, 0},
        {0, 0, 0, 0},
    }
};


Camera camera = {
    .position = {0, 0, 1.8f},
    .rotation = {0, 0, -3.14f},
    .scale = {200, 200, 200},
};


bool LOTRI_RenewCamera() {
    {
        const float angle = 0.03f;
        if (DEVICE_KeyPressed(SDL_SCANCODE_LEFT)) camera.rotation.v.z += angle;
        else if (DEVICE_KeyPressed(SDL_SCANCODE_RIGHT)) camera.rotation.v.z -= angle;
        else if (DEVICE_KeyPressed(SDL_SCANCODE_DOWN)) camera.rotation.v.y += angle;
        else if (DEVICE_KeyPressed(SDL_SCANCODE_UP)) camera.rotation.v.y -= angle;

        if (camera.rotation.v.y < -1.57f) camera.rotation.v.y = -1.57f;
        if (camera.rotation.v.y > +1.57f) camera.rotation.v.y = +1.57f;
    }
    {
        const float step = 0.01f;
        if (DEVICE_KeyPressed(SDL_SCANCODE_W)) {
            camera.position.v.x += step * SDL_cosf(camera.rotation.v.z);
            camera.position.v.y += step * SDL_sinf(camera.rotation.v.z);
        }
        else if (DEVICE_KeyPressed(SDL_SCANCODE_S)) {
            camera.position.v.x -= step * SDL_cosf(camera.rotation.v.z);
            camera.position.v.y -= step * SDL_sinf(camera.rotation.v.z);
        }
        else if (DEVICE_KeyPressed(SDL_SCANCODE_A)) {
            camera.position.v.x -= step * SDL_sinf(camera.rotation.v.z);
            camera.position.v.y += step * SDL_cosf(camera.rotation.v.z);
        }
        else if (DEVICE_KeyPressed(SDL_SCANCODE_D)) {
            camera.position.v.x += step * SDL_sinf(camera.rotation.v.z);
            camera.position.v.y -= step * SDL_cosf(camera.rotation.v.z);
        }
    }
    Mat4f matArr[] = {
        LOTRI_GetInvT(camera.position),
        LOTRI_GetInvR(camera.rotation),
        LOTRI_GetMatS(camera.scale),
        matProj,
    };
    camera.mat = LOTRI_GetProd(len_of(matArr), matArr);

    DEBUG_SendMessageL("%s:\n", __func__);
    DEBUG_SendMessageL("    p: %.2f, %.2f, %.2f\n", camera.position.v.x, camera.position.v.y, camera.position.v.z);
    DEBUG_SendMessageL("    r: %.2f, %.2f, %.2f\n", camera.rotation.v.x, camera.rotation.v.y, camera.rotation.v.z);

    return true;
}