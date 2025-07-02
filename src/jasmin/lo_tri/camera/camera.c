#include "camera.h"


Camera camera = {(Vec3f){1, 0, 0}, (Vec3f){0}};


void LOTRI_RenewCamera() {
    if (keyPressed(SDL_SCANCODE_W)) {
        camera.position.x -= 1;
    }
    if (keyPressed(SDL_SCANCODE_S)) {
        camera.position.x += 1;
    }
    if (keyPressed(SDL_SCANCODE_A)) {
        camera.position.y -= 1;
    }
    if (keyPressed(SDL_SCANCODE_D)) {
        camera.position.y += 1;
    }
    DEBUG_SendMessage("Camera{%d}\n", keyPressed(SDL_SCANCODE_LEFT));
    DEBUG_SendMessage("Pos: %.2f, %.2f, %.2f\n", camera.position.x, camera.position.y, camera.position.z);
    DEBUG_SendMessage("Rot: %.2f, %.2f, %.2f", camera.rotation.x, camera.rotation.y, camera.rotation.z);
}