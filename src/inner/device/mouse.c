#include "mouse.h"


typedef struct MouseState {
    SDL_FPoint pos, leftPos, rightPos;
    bool leftPressed, rightPressed;
} MouseState;
typedef struct Mouse {
    MouseState state1, state2;
    const Trig* left_trig;
} Mouse;

Mouse mouse = {0};


// SET & GET ===========================================================================================================
bool SLD_GetPointInRect(const SDL_FPoint point, const SDL_FRect rect) {
    return rect.x <= point.x && point.x < rect.x + rect.w && rect.y <= point.y && point.y < rect.y + rect.h;
}
SDL_FPoint DEVICE_GetMousePos() {
    return mouse.state2.pos;
}
void DEVICE_SetMouseLeftTrig(const Trig *trig) {
    mouse.left_trig = trig;
}
bool DEVICE_GetMouseLeftInRect(const SDL_FRect rect) {
    return SLD_GetPointInRect(mouse.state2.leftPos, rect);
}
bool DEVICE_GetMouseInRect(const SDL_FRect rect) {
    return SLD_GetPointInRect(mouse.state2.pos, rect);
}


// RENEW ===============================================================================================================
bool DEVICE_RenewMouse() {
    float x, y;
    const SDL_MouseButtonFlags buttons = SDL_GetMouseState(&x, &y);

    if (mouse.left_trig != NULL
        && mouse.left_trig->sustain == false
        && mouse.state2.leftPressed == true
        && (buttons & SDL_BUTTON_LMASK) == false
        ) {
        ma_engine_play_sound(&engine, "../res/sound/switch.wav", NULL);
        PullTrig(mouse.left_trig);
        }

    if (mouse.left_trig != NULL
        && mouse.left_trig->sustain == true
        && mouse.state2.leftPressed == true
        ) {
        if ((buttons & SDL_BUTTON_LMASK) == false)
            ma_engine_play_sound(&engine, "../res/sound/switch.wav", NULL);
        PullTrig(mouse.left_trig);
        }
    mouse.left_trig = NULL;
    mouse.state1 = mouse.state2;
    mouse.state2.pos = (SDL_FPoint){x * scale_x, y * scale_y};

    if (!mouse.state2.leftPressed && buttons & SDL_BUTTON_LMASK) {
        mouse.state2.leftPos = mouse.state2.pos;
    }
    else if (mouse.state2.leftPressed && !(buttons & SDL_BUTTON_LMASK)) {
        mouse.state2.leftPos = (SDL_FPoint){-255, -245};
    }
    else if (!mouse.state2.leftPressed) {
        mouse.state2.leftPos = (SDL_FPoint){-255, -245};
    }
    mouse.state2.leftPressed = buttons & SDL_BUTTON_LMASK;
    mouse.state2.rightPressed = buttons & SDL_BUTTON_RMASK;
    return true;
}


// DRAW ================================================================================================================
bool DEVICE_DrawMouse() {
    DEBUG_SendMessageL("mouse: %.2f, %.2f\n", mouse.state2.pos.x, mouse.state2.pos.y);
    if (mouse.left_trig != NULL) {
        DEBUG_SendMessageL("mouse.left_trig != NULL\n");
    }
    DEBUG_DrawPoint(mouse.state2.leftPos.x, mouse.state2.leftPos.y);
    DEBUG_DrawPoint(mouse.state2.pos.x, mouse.state2.pos.y);
    if (mouse.state2.leftPressed) {
        DEBUG_DrawLine(
            mouse.state2.leftPos.x, mouse.state2.leftPos.y,
            mouse.state2.pos.x, mouse.state2.pos.y
            );
    }
    return true;
}
