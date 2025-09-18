#ifndef JASMIN_BASIC_DEVICE_H
#define JASMIN_BASIC_DEVICE_H


#include "_device.h"
#include "mouse.h"
#include "keyboard.h"


bool DEVICE_Renew();
bool DEVICE_Draw();

void DEVICE_RenewKeyboard();
bool DEVICE_KeyPressed(SDL_Scancode key);


#endif //JASMIN_BASIC_DEVICE_H