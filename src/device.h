#ifndef DEVICE_H
#define DEVICE_H


#pragma region MOUSE ===================================================================================================


struct {
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
        mouse.left_y = -255;
    } else if (!mouse.left_pressed) {
        mouse.left_x = -255;
        mouse.left_y = -255;
    }
    mouse.left_pressed = buttons & SDL_BUTTON_LMASK;
    mouse.right_pressed = buttons & SDL_BUTTON_RMASK;
}
void drawMouse(SDL_Renderer* renderer) {
    DEBUG_DrawPoint(renderer, mouse.x, mouse.y);
    DEBUG_DrawPoint(renderer, mouse.left_x, mouse.left_y);
}
bool mouseInRect(const SDL_FRect* rect) {
    return (
        rect->x <= mouse.x && mouse.x < rect->x + rect->w &&
        rect->y <= mouse.y && mouse.y < rect->y + rect->h
        );
}


#pragma endregion MOUSE ================================================================================================
#pragma region KEYBOARD ================================================================================================


struct {} keyboard;


void renewKeyboard() {
    //
}


bool keyInKeyBoard(SDL_Scancode key) {
    //
    return false;
}


#pragma endregion KEYBOARD =============================================================================================


#endif //DEVICE_H