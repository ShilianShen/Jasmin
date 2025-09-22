#ifndef KEYBOARD_H
#define KEYBOARD_H


#include "_device.h"


bool DEVICE_RenewKeyboard();
bool DEVICE_DrawKeyboard();
bool DEVICE_GetKeyPressed(SDL_Scancode key);
bool DEVICE_GetKeyPress(SDL_Scancode key);


#endif //KEYBOARD_H