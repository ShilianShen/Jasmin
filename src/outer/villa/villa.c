#include "villa.h"
#include "weather.h"



bool VILLA_Init() {
    VILLA_InitRain();
    return true;
}
bool VILLA_Renew() {
    VILLA_RenewRain();
    return true;
}
bool VILLA_Draw() {
    VILLA_DrawRain();
    return true;
}
void VILLA_Exit() {

}
