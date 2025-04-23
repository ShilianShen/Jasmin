#ifndef DEVICE_H
#define DEVICE_H




// display parameters
int logical_w, logical_h;
int physical_w, physical_h;
float scale_x = 1, scale_y = 1;

void renewScreenScale(SDL_Window* window) {
    SDL_GetWindowSize(window, &logical_w, &logical_h);
    SDL_GL_GetDrawableSize(window, &physical_w, &physical_h);
    scale_x = (float)physical_w / (float)logical_w;
    scale_y = (float)physical_h / (float)logical_h;
}


// mouse
struct {
    int x, y;

    // left
    bool left_pressed;
    int left_x, left_y;

    // right
    bool right_pressed;
    int right_x, right_y;
} mouse;
void renewMouse() {
    const Uint32 buttons = SDL_GetMouseState(&mouse.x, &mouse.y);
    mouse.x *= scale_x;
    mouse.y *= scale_y;

    if (!mouse.left_pressed && buttons & SDL_BUTTON(SDL_BUTTON_LEFT)) {
        mouse.left_x = mouse.x;
        mouse.left_y = mouse.y;
    } else if (mouse.left_pressed && !buttons & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
        mouse.left_x = 0;
        mouse.left_y = 0;
    }
    mouse.left_pressed = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
    mouse.right_pressed = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);
}
void drawMouse(SDL_Renderer* renderer) {
    DEBUG_DrawPoint(renderer, mouse.x, mouse.y);
    DEBUG_DrawPoint(renderer, mouse.left_x, mouse.left_y);
}
bool mouseInRect(const SDL_Rect* rect) {
    return (
        rect->x <= mouse.x && mouse.x < rect->x + rect->w &&
        rect->y <= mouse.y && mouse.y < rect->y + rect->h
        );
}




// keyboard
struct {} keyboard;
void renewKeyboard() {
    //
}
bool keyInKeyBoard(SDL_Scancode key) {
    //
    return false;
}




#endif //DEVICE_H