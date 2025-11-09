#include "perph.h"


bool PERPH_Init() {
    return true
    && PERPH_InitMouse()
    ;
}
void PERPH_Exit() {
    PERPH_ExitMouse();
}
bool PERPH_Renew() {
    REQ_CONDITION(PERPH_RenewMouse(), return false);
    REQ_CONDITION(PERPH_RenewBoard(), return false);
    return true;
}
bool PERPH_Draw() {
    REQ_CONDITION(PERPH_DrawMouse(), return false);
    REQ_CONDITION(PERPH_DrawBoard(), return false);
    return true;
}


