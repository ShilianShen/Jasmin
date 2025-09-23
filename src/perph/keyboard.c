#include "keyboard.h"


static struct {
    struct {
        bool pressed[SDL_SCANCODE_COUNT];
    } state1, state2;
} keyboard;


// SET & GET ===========================================================================================================
bool PERPH_GetKeyPressed(const SDL_Scancode key) {
    return keyboard.state2.pressed[key];
}
bool PERPH_GetKeyPress(const SDL_Scancode key) {
    return keyboard.state1.pressed[key] == false && keyboard.state2.pressed[key] == true;
}


// RENEW ===============================================================================================================
bool PERPH_RenewKeyboard() {
    const bool* state = SDL_GetKeyboardState(NULL);
    keyboard.state1 = keyboard.state2;
    for (int i = 0; i < SDL_SCANCODE_COUNT; i++) {
        keyboard.state2.pressed[i] = state[i];
    }
    return true;
}


// DRAW ================================================================================================================
bool PERPH_DrawKeyboard() {
    return true;
}
