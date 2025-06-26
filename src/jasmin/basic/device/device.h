#ifndef JASMIN_BASIC_DEVICE_H
#define JASMIN_BASIC_DEVICE_H


#include "../debug/debug.h"


void renewMouse();
void drawMouse(SDL_Renderer* renderer);
bool mouseInRect(const SDL_FRect* rect);
bool mouseLeftInRect(const SDL_FRect* rect);


void renewKeyboard();
bool keyInKeyBoard(SDL_Scancode key);


#endif //JASMIN_BASIC_DEVICE_H