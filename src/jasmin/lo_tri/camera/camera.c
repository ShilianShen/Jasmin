#include "camera.h"


Camera camera = {(Vec3f){-100, 0, 0}, (Vec3f){0}};


void LOTRI_RenewCamera() {
    const float r = 0.001f, t = 0.1f;

    camera.rotation.z += r * (float)(keyPressed(SDL_SCANCODE_LEFT) - keyPressed(SDL_SCANCODE_RIGHT));
    camera.rotation.y += r * (float)(keyPressed(SDL_SCANCODE_DOWN) - keyPressed(SDL_SCANCODE_UP));
    camera.rotation.x += r * (float)(keyPressed(SDL_SCANCODE_PERIOD) - keyPressed(SDL_SCANCODE_COMMA));

    camera.translation.x += t * (float)(keyPressed(SDL_SCANCODE_W) - keyPressed(SDL_SCANCODE_S));
    camera.translation.y += t * (float)(keyPressed(SDL_SCANCODE_A) - keyPressed(SDL_SCANCODE_D));
    camera.translation.z += t * (float)(keyPressed(SDL_SCANCODE_R) - keyPressed(SDL_SCANCODE_F));

    DEBUG_SendMessageL("camera.pos: %.2f, %.2f, %.2f\n", camera.translation.x, camera.translation.y, camera.translation.z);
    DEBUG_SendMessageL("camera.rot: %.2f, %.2f, %.2f\n", camera.rotation.x, camera.rotation.y, camera.rotation.z);
}