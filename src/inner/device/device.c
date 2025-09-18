#include "device.h"


bool DEVICE_Renew() {
    return true
    && DEVICE_RenewMouse()
    && DEVICE_RenewKeyboard()
    ;
}
bool DEVICE_Draw() {
    return true
    && DEVICE_DrawMouse()
    && DEVICE_DrawKeyboard()
    ;
}


