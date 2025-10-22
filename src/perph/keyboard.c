#include "keyboard.h"


static struct {
    struct {
        bool pressed[SDL_SCANCODE_COUNT];
    } state1, state2;
    Trig trigArray[SDL_SCANCODE_COUNT];
} keyboard;


// SET & GET ===========================================================================================================
bool PERPH_GetKeyPressed(const SDL_Scancode key) {
    return keyboard.state2.pressed[key];
}
bool PERPH_GetKeyPress(const SDL_Scancode key) {
    return keyboard.state1.pressed[key] == false && keyboard.state2.pressed[key] == true;
}
bool PERPH_SetKeyTrig(const SDL_Scancode key, const Trig trig) {
    keyboard.trigArray[key] = trig;
    return true;
}


// RENEW ===============================================================================================================
static bool PERPH_RenewKeyboard_State() {
    const bool* state = SDL_GetKeyboardState(NULL);
    keyboard.state1 = keyboard.state2;
    for (int i = 0; i < SDL_SCANCODE_COUNT; i++) {
        keyboard.state2.pressed[i] = state[i];
    }
    return true;
}
static bool PERPH_RenewKeyboard_TrigArray() {
    for (int i = 0; i < SDL_SCANCODE_COUNT; i++) {
        if (keyboard.trigArray[i].func == NULL) continue;
        const Trig trig = keyboard.trigArray[i];
        if (trig.sustain) {
            if (keyboard.state2.pressed[i]) {
                BASIC_PullTrig(trig);
            }
        }
        else {
            if (keyboard.state1.pressed[i] == false && keyboard.state2.pressed[i] == true) {
                BASIC_PullTrig(trig);
            }
        }
    }
    return true;
}
bool PERPH_RenewKeyboard() {
    return true
    && PERPH_RenewKeyboard_State()
    && PERPH_RenewKeyboard_TrigArray()
    ;
}


// DRAW ================================================================================================================
bool PERPH_DrawKeyboard() {
    DEBUG_SendMessageL("%s: [", __func__);
    for (int i = 0; i < SDL_SCANCODE_COUNT; i++) {
        if (keyboard.state2.pressed[i] == true && keyboard.trigArray[i].func != NULL) {
            DEBUG_SendMessageL("%s, ", SDL_GetScancodeName(i));
        }
    }
    DEBUG_SendMessageL("]\n", __func__);
    return true;
}