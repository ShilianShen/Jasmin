#include "device.h"


bool DEVICE_Init() {
    return true;
}
void DEVICE_Exit() {
}
bool DEVICE_Renew() {
    REQ_CONDITION(DEVICE_RenewMouse(), return false);
    REQ_CONDITION(DEVICE_RenewKeyboard(), return false);
    return true;
}
bool DEVICE_Draw() {
    REQ_CONDITION(DEVICE_DrawMouse(), return false);
    REQ_CONDITION(DEVICE_DrawKeyboard(), return false);
    return true;
}


