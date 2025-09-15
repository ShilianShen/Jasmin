#include "camera.h"


Camera camera = {
    .position = {1, 0, 0},
    .rotation = {0, 0, -3.14f},
};


bool LOTRI_RenewCamera() {
    const float angle = 0.03f;
    if (DEVICE_KeyPressed(SDL_SCANCODE_LEFT )) camera.rotation.v.z += angle;
    if (DEVICE_KeyPressed(SDL_SCANCODE_RIGHT)) camera.rotation.v.z -= angle;
    if (DEVICE_KeyPressed(SDL_SCANCODE_DOWN )) camera.rotation.v.y += angle;
    if (DEVICE_KeyPressed(SDL_SCANCODE_UP   )) camera.rotation.v.y -= angle;
    DEBUG_SendMessageL("%s:\n", __func__);
    DEBUG_SendMessageL("    p: %.2f, %.2f, %.2f\n", camera.position.v.x, camera.position.v.y, camera.position.v.z);
    DEBUG_SendMessageL("    r: %.2f, %.2f, %.2f\n", camera.rotation.v.x, camera.rotation.v.y, camera.rotation.v.z);
    return true;
}