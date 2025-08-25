#ifndef JASMIN_BASIC_DEVICE_H
#define JASMIN_BASIC_DEVICE_H


#include "../debug/debug.h"


bool DEVICE_Renew();

bool DEVICE_Draw();
bool DEVICE_MouseInRect(const SDL_FRect* rect);
bool DEVICE_MouseLeftInRect(const SDL_FRect* rect);
void DEVICE_SetMouseLeftTrig(const Trig *trig);


SDL_FPoint DEVICE_GetMousePos();
void DEVICE_RenewKeyboard();
bool DEVICE_KeyPressed(SDL_Scancode key);


#endif //JASMIN_BASIC_DEVICE_H