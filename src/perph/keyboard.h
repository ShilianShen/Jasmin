#ifndef PERPH_KEYBOARD_H
#define PERPH_KEYBOARD_H


#include "_perph.h"


bool PERPH_RenewKeyboard();
bool PERPH_DrawKeyboard();
bool PERPH_GetKeyPressed(SDL_Scancode key);
bool PERPH_GetKeyPress(SDL_Scancode key);


#endif //PERPH_KEYBOARD_H