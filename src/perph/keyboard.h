#ifndef PERPH_KEYBOARD_H
#define PERPH_KEYBOARD_H


#include "_perph.h"


bool PERPH_RenewKeyboard();
bool PERPH_DrawKeyboard();
bool PERPH_GetKeyPressed(SDL_Scancode key);
bool PERPH_GetKeyPress(SDL_Scancode key);
bool PERPH_SetKeyTrig(SDL_Scancode key, const Trig* trig);


#endif //PERPH_KEYBOARD_H