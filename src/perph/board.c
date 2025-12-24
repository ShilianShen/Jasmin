#include "board.h"


static struct {
    struct {
        bool pressed[SDL_SCANCODE_COUNT];
    } state1, state2;
    Trig trigArray[SDL_SCANCODE_COUNT];
} board;


// GET & SET ===========================================================================================================
bool PERPH_GetBoardKeyPressed(const SDL_Scancode key) {
    return board.state2.pressed[key];
}
bool PERPH_GetBoardKeyPress(const SDL_Scancode key) {
    return board.state1.pressed[key] == false && board.state2.pressed[key] == true;
}
bool PERPH_SetBoardKeyTrig(const SDL_Scancode key, const Trig trig) {
    board.trigArray[key] = trig;
    return true;
}
bool PERPH_ResetBoardKeyTrig() {
    for (int i = 0; i < SDL_SCANCODE_COUNT; i++) {
        board.trigArray[i].func = NULL;
    }
    return true;
}


// RENEW ===============================================================================================================
static bool PERPH_RenewKeyboard_State() {
    const bool* state = SDL_GetKeyboardState(NULL);
    board.state1 = board.state2;
    for (int i = 0; i < SDL_SCANCODE_COUNT; i++) {
        board.state2.pressed[i] = state[i];
    }
    return true;
}
static bool PERPH_RenewKeyboard_TrigArray() {
    for (int i = 0; i < SDL_SCANCODE_COUNT; i++) {
        if (board.trigArray[i].func == NULL) continue;
        const Trig trig = board.trigArray[i];
        if (trig.sustain) {
            if (board.state2.pressed[i]) {
                BASIC_PullTrig(trig);
            }
        }
        else {
            if (board.state1.pressed[i] == true && board.state2.pressed[i] == false) {
                BASIC_PullTrig(trig);
            }
        }
    }
    return true;
}
bool PERPH_RenewBoard() {
    return true
    && PERPH_RenewKeyboard_State()
    && PERPH_RenewKeyboard_TrigArray()
    && PERPH_ResetBoardKeyTrig()
    ;
}


// DRAW ================================================================================================================
bool PERPH_DrawBoard() {
    DEBUG_SendMessageL("%s: [", __func__);
    for (int i = 0; i < SDL_SCANCODE_COUNT; i++) {
        if (board.trigArray[i].func == NULL) continue;

        if (board.state2.pressed[i] == true) {
            DEBUG_SendMessageL("[%s], ", SDL_GetScancodeName(i));
        }
        else DEBUG_SendMessageL("%s, ", SDL_GetScancodeName(i));
    }
    DEBUG_SendMessageL("]\n", __func__);
    return true;
}