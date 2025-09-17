#include "lotri.h"


bool LOTRI_Init() {
    return true;
}
bool LOTRI_Renew() {
    LOTRI_RenewCamera();
    return true;
}
bool LOTRI_Draw() {
    return true;
}
void LOTRI_Exit() {
}