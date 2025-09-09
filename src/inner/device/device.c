#include "device.h"



struct Mouse {
    float x, y;
    // left
    bool left_pressed;
    float left_x, left_y;
    const Trig* left_trig;
    // right
    bool right_pressed;
    float right_x, right_y;
};
struct Mouse mouse = {0};


bool DEVICE_Renew() {
    const SDL_MouseButtonFlags buttons = SDL_GetMouseState(&mouse.x, &mouse.y);

    if (mouse.left_trig != NULL
        && mouse.left_trig->sustain == false
        && mouse.left_pressed == true
        && (buttons & SDL_BUTTON_LMASK) == false
        ) {
        ma_engine_play_sound(&engine, "../res/sound/switch.wav", NULL);
        PullTrig(mouse.left_trig);
    }

    if (mouse.left_trig != NULL
        && mouse.left_trig->sustain == true
        && mouse.left_pressed == true
        ) {
        ma_engine_play_sound(&engine, "../res/sound/switch.wav", NULL);
        PullTrig(mouse.left_trig);
    }
    mouse.left_trig = NULL;

    mouse.x *= scale_x;
    mouse.y *= scale_y;
    if (!mouse.left_pressed && buttons & SDL_BUTTON_LMASK) {
        mouse.left_x = mouse.x;
        mouse.left_y = mouse.y;
    } else if (mouse.left_pressed && !(buttons & SDL_BUTTON_LMASK)) {
        mouse.left_x = -255;
        mouse.left_y = -245;
    } else if (!mouse.left_pressed) {
        mouse.left_x = -255;
        mouse.left_y = -255;
    }
    mouse.left_pressed = buttons & SDL_BUTTON_LMASK;
    mouse.right_pressed = buttons & SDL_BUTTON_RMASK;
    return true;
}

bool DEVICE_Draw() {
    DEBUG_SendMessageL("mouse: %.2f, %.2f\n", mouse.x, mouse.y);
    if (mouse.left_trig != NULL) {
        DEBUG_SendMessageL("mouse.left_trig != NULL\n");
    }
    DEBUG_DrawPoint(mouse.left_x, mouse.left_y);
    DEBUG_DrawPoint(mouse.x, mouse.y);
    if (mouse.left_pressed) {
        DEBUG_DrawLine(mouse.x, mouse.y, mouse.left_x, mouse.left_y);
    }
    return true;
}
bool DEVICE_MouseInRect(const SDL_FRect* rect) {
    return (
        rect->x <= mouse.x && mouse.x < rect->x + rect->w &&
        rect->y <= mouse.y && mouse.y < rect->y + rect->h
        );
}
bool DEVICE_MouseLeftInRect(const SDL_FRect* rect) {
    return (
        rect->x <= mouse.left_x && mouse.left_x < rect->x + rect->w &&
        rect->y <= mouse.left_y && mouse.left_y < rect->y + rect->h
        );
}
void DEVICE_SetMouseLeftTrig(const Trig *trig) {
    mouse.left_trig = trig;
}



SDL_FPoint DEVICE_GetMousePos() {
    const SDL_FPoint point = {mouse.x, mouse.y};
    return point;
}


struct Keyboard {} keyboard;


void DEVICE_RenewKeyboard() {
    //
}
bool DEVICE_KeyPressed(const SDL_Scancode key) {
    const bool* state = SDL_GetKeyboardState(NULL);
    //
    if (state[key]) {
        return true;
    }
    return false;
}
