#include "villa.h"
#include "weather.h"



bool VILLA_Init() {
    LOTRI_InitCube();
    VILLA_InitRain();
    return true;
}
bool VILLA_Renew() {
    LOTRI_RenewModel(cube);
    VILLA_RenewRain();
    return true;
}
bool VILLA_Draw() {
    LOTRI_DrawModel(cube);
    // VILLA_DrawRain();
    return true;
}
void VILLA_Exit() {
    LOTRI_DestroyModel(cube);
}
