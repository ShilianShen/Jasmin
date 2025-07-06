#include "camera.h"


Camera camera = {(Vec3f){0}, (Vec3f){0}};


void LOTRI_RenewCamera() {
    const float r = 0.05f, t = 5.f;
    if (keyPressed(SDL_SCANCODE_UP)) {
        camera.rotation.y += r;
    }
    if (keyPressed(SDL_SCANCODE_DOWN)) {
        camera.rotation.y -= r;
    }
    if (keyPressed(SDL_SCANCODE_LEFT)) {
        camera.rotation.x += r;
    }
    if (keyPressed(SDL_SCANCODE_RIGHT)) {
        camera.rotation.x -= r;
    }
    if (keyPressed(SDL_SCANCODE_COMMA)) {
        camera.rotation.z += r;
    }
    if (keyPressed(SDL_SCANCODE_PERIOD)) {
        camera.rotation.z -= r;
    }


    if (keyPressed(SDL_SCANCODE_W)) {
        camera.translation.x -= t;
    }
    if (keyPressed(SDL_SCANCODE_S)) {
        camera.translation.x += t;
    }
    if (keyPressed(SDL_SCANCODE_A)) {
        camera.translation.y -= t;
    }
    if (keyPressed(SDL_SCANCODE_D)) {
        camera.translation.y += t;
    }
    if (keyPressed(SDL_SCANCODE_R)) {
        camera.translation.z += t;
    }
    if (keyPressed(SDL_SCANCODE_F)) {
        camera.translation.z -= t;
    }
    DEBUG_SendMessageL("camera.pos: %.2f, %.2f, %.2f\n", camera.translation.x, camera.translation.y, camera.translation.z);
    DEBUG_SendMessageL("camera.rot: %.2f, %.2f, %.2f\n", camera.rotation.x, camera.rotation.y, camera.rotation.z);
}