#include "perph.h"


bool PERPH_Init() {
    return true;
}
void PERPH_Exit() {
}
bool PERPH_Renew() {
    REQ_CONDITION(PERPH_RenewMouse(), return false);
    REQ_CONDITION(PERPH_RenewKeyboard(), return false);
    return true;
}
bool PERPH_Draw() {
    REQ_CONDITION(PERPH_DrawMouse(), return false);
    REQ_CONDITION(PERPH_DrawKeyboard(), return false);
    return true;
}


