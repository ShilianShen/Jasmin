#include "lotri.h"


bool LOTRI_Init() {
    return true;
}
bool LOTRI_Renew() {
    LOTRI_ClearBuffer();
    return true;
}
bool LOTRI_Draw() {
    LOTRI_RenewCamera();
    LOTRI_DrawCamera();
    LOTRI_DrawBuffer();
    return true;
}
void LOTRI_Exit() {
}