#include "mouse.h"


static struct Mouse {
    struct {
        SDL_FPoint pos, leftPos, rightPos;
        bool leftPressed, rightPressed;
    } state1, state2;
    const Trig* left_trig;
} mouse;


// SET & GET ===========================================================================================================
bool SLD_GetPointInRect(const SDL_FPoint point, const SDL_FRect rect) {
    return rect.x <= point.x && point.x < rect.x + rect.w && rect.y <= point.y && point.y < rect.y + rect.h;
} // remember to put it in BASIC
SDL_FPoint PERPH_GetMousePos() {
    return mouse.state2.pos;
}
void PERPH_SetMouseLeftTrig(const Trig *trig) {
    mouse.left_trig = trig;
}
bool PERPH_GetMouseLeftInRect(const SDL_FRect rect) {
    return SLD_GetPointInRect(mouse.state2.leftPos, rect);
}
bool PERPH_GetMouseInRect(const SDL_FRect rect) {
    return SLD_GetPointInRect(mouse.state2.pos, rect);
}


// RENEW ===============================================================================================================
static void PERPH_RenewMouse_State() {
    mouse.state1 = mouse.state2;

    float x, y;
    const SDL_MouseButtonFlags buttons = SDL_GetMouseState(&x, &y);
    mouse.state2.pos = (SDL_FPoint){x * scale_x, y * scale_y};
    mouse.state2.leftPressed = buttons & SDL_BUTTON_LMASK;
    mouse.state2.rightPressed = buttons & SDL_BUTTON_RMASK;

    if (!mouse.state1.leftPressed && mouse.state2.leftPressed)
        mouse.state2.leftPos = mouse.state2.pos;
    else if (mouse.state1.leftPressed && !mouse.state2.leftPressed)
        mouse.state2.leftPos = (SDL_FPoint){-255, -245};
}
static void PERPH_RenewMouse_Trig() {
    if (mouse.left_trig != NULL
        && mouse.left_trig->sustain == false
        && mouse.state1.leftPressed == true
        && mouse.state2.leftPressed == false
        ) {
        ma_engine_play_sound(&engine, "../res/sound/switch.wav", NULL);
        PullTrig(mouse.left_trig);
        }
    if (mouse.left_trig != NULL
        && mouse.left_trig->sustain == true
        && mouse.state1.leftPressed == true
        ) {
        if (mouse.state2.leftPressed == false)
            ma_engine_play_sound(&engine, "../res/sound/switch.wav", NULL);
        PullTrig(mouse.left_trig);
        }
    mouse.left_trig = NULL;
}
bool PERPH_RenewMouse() {
    PERPH_RenewMouse_State();
    PERPH_RenewMouse_Trig();
    return true;
}


// DRAW ================================================================================================================
bool PERPH_DrawMouse() {
    DEBUG_SendMessageL("mouse: %.2f, %.2f\n", mouse.state2.pos.x, mouse.state2.pos.y);
    if (mouse.left_trig != NULL) {
        DEBUG_SendMessageL("mouse.left_trig != NULL\n");
    }
    DEBUG_DrawPoint(mouse.state2.leftPos);
    DEBUG_DrawPoint(mouse.state2.pos);
    if (mouse.state2.leftPressed) {
        DEBUG_DrawLine(
            mouse.state2.leftPos,
            mouse.state2.pos
            );
    }
    return true;
}
