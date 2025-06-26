#include "device.h"


struct Mouse {
    float x, y;
    // left
    bool left_pressed;
    float left_x, left_y;
    // right
    bool right_pressed;
    float right_x, right_y;
} mouse;


void renewMouse() {
    const SDL_MouseButtonFlags buttons = SDL_GetMouseState(&mouse.x, &mouse.y);
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
}
void drawMouse(SDL_Renderer* renderer) {
    static char text[32];

    snprintf(text, 32, "%.f, %.f", mouse.x, mouse.y);
    if (DEBUG_HaveMessage()) {
        DEBUG_DrawMessage(mouse.x, mouse.y);
    }
    else {
        DEBUG_DrawText(mouse.x, mouse.y, text);
    }

    DEBUG_DrawPoint(mouse.left_x, mouse.left_y);
    DEBUG_DrawPoint(mouse.x, mouse.y);
    if (mouse.left_pressed) {
        DEBUG_DrawLine(mouse.x, mouse.y, mouse.left_x, mouse.left_y);
    }
}
bool mouseInRect(const SDL_FRect* rect) {
    return (
        rect->x <= mouse.x && mouse.x < rect->x + rect->w &&
        rect->y <= mouse.y && mouse.y < rect->y + rect->h
        );
}
bool mouseLeftInRect(const SDL_FRect* rect) {
    return (
        rect->x <= mouse.left_x && mouse.left_x < rect->x + rect->w &&
        rect->y <= mouse.left_y && mouse.left_y < rect->y + rect->h
        );
}


struct Keyboard {} keyboard;


void renewKeyboard() {
    //
}
bool keyInKeyBoard(SDL_Scancode key) {
    //
    return false;
}
