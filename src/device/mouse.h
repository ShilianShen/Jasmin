#ifndef MOUSE_H
#define MOUSE_H


#include "_device.h"


bool DEVICE_GetMouseInRect(SDL_FRect rect);
bool DEVICE_GetMouseLeftInRect(SDL_FRect rect);
void DEVICE_SetMouseLeftTrig(const Trig *trig);
SDL_FPoint DEVICE_GetMousePos();


bool DEVICE_DrawMouse();
bool DEVICE_RenewMouse();


#endif //MOUSE_H