#ifndef PERPH_MOUSE_H
#define PERPH_MOUSE_H


#include "_perph.h"


bool PERPH_InitMouse();
bool PERPH_DrawMouse();
bool PERPH_RenewMouse();
void PERPH_ExitMouse();


bool PERPH_GetMouseInRect(SDL_FRect rect);
bool PERPH_GetMouseLeftInRect(SDL_FRect rect);
void PERPH_SetMouseLeftTrig(const Trig *trig);
SDL_FPoint PERPH_GetMousePos();


#endif //PERPH_MOUSE_H