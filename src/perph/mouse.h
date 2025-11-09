#ifndef PERPH_MOUSE_H
#define PERPH_MOUSE_H


#include "_perph.h"


typedef enum PERPH_MouseKey {
    PERPH_MOUSE_KEY_LEFT,
    PERPH_MOUSE_KEY_RIGHT,
    PERPH_NUM_MOUSE_KEYS,
} PERPH_MouseKey;


bool PERPH_InitMouse();
bool PERPH_DrawMouse();
bool PERPH_RenewMouse();
void PERPH_ExitMouse();


bool PERPH_GetMouseInRect(SDL_FRect rect);
bool PERPH_GetMouseKeyInRect(PERPH_MouseKey key, SDL_FRect rect);
bool PERPH_GetMouseAndKeyInRect(PERPH_MouseKey key, SDL_FRect rect);
bool PERPH_GetMouseKeyPressed(PERPH_MouseKey key);
bool PERPH_GetMouseKeyPress(PERPH_MouseKey key);
bool PERPH_SetMouseKeyTrig(PERPH_MouseKey key, Trig trig);

SDL_FPoint PERPH_GetMousePos();


#endif //PERPH_MOUSE_H