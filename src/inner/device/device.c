#include "device.h"






bool DEVICE_Renew() {
    DEVICE_RenewMouse();
    return true;
}
bool DEVICE_Draw() {
    DEVICE_DrawMouse();
    return true;
}


struct Keyboard {
    bool state1[SDL_SCANCODE_COUNT];
    bool state2[SDL_SCANCODE_COUNT];
} keyboard;


void DEVICE_RenewKeyboard() {

}
bool DEVICE_KeyPressed(const SDL_Scancode key) {
    const bool* state = SDL_GetKeyboardState(NULL);

    if (state[key]) {
        return true;
    }
    return false;
}
