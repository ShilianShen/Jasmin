#include "keyboard.h"


typedef struct KeyboardState {
    bool pressed[SDL_SCANCODE_COUNT];
} KeyboardState;

typedef struct Keyboard {
    KeyboardState state1, state2;
} Keyboard;

Keyboard keyboard;


// SET & GET ===========================================================================================================
bool DEVICE_GetKeyPressed(const SDL_Scancode key) {
    return keyboard.state2.pressed[key];
}
bool DEVICE_GetKeyPress(const SDL_Scancode key) {
    return keyboard.state1.pressed[key] == false && keyboard.state2.pressed[key] == true;
}


// RENEW ===============================================================================================================
bool DEVICE_RenewKeyboard() {
    const bool* state = SDL_GetKeyboardState(NULL);
    keyboard.state1 = keyboard.state2;
    for (int i = 0; i < SDL_SCANCODE_COUNT; i++) {
        keyboard.state2.pressed[i] = state[i];
    }
    return true;
}


// DRAW ================================================================================================================
bool DEVICE_DrawKeyboard() {
    return true;
}
