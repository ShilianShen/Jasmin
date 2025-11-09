#ifndef PERPH_KEYBOARD_H
#define PERPH_KEYBOARD_H


#include "_perph.h"


bool PERPH_RenewBoard();
bool PERPH_DrawBoard();
bool PERPH_GetBoardKeyPressed(SDL_Scancode key);
bool PERPH_GetBoardKeyPress(SDL_Scancode key);
bool PERPH_SetBoardKeyTrig(SDL_Scancode key, Trig trig);
bool PERPH_ResetBoardKeyTrig();


#endif //PERPH_KEYBOARD_H