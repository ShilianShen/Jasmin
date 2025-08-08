#ifndef JASMIN_BASIC_DEVICE_H
#define JASMIN_BASIC_DEVICE_H


#include "../debug/debug.h"


void DEVICE_RenewMouse();
void DEVICE_DrawMouse(SDL_Renderer* renderer);
bool DEVICE_MouseInRect(const SDL_FRect* rect);
bool DEVICE_MouseLeftInRect(const SDL_FRect* rect);


void DEVICE_RenewKeyboard();
bool DEVICE_KeyPressed(SDL_Scancode key);


#endif //JASMIN_BASIC_DEVICE_H