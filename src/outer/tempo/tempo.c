#include "tempo.h"


bool TEMPO_Load() {
    TEMPO_InitMenu();
    TEMPO_LoadBasic();
    TEMPO_LoadMenu();
    return true;
}
void TEMPO_Unload() {
    TEMPO_UnloadMenu();
    TEMPO_UnloadBasic();
    TEMPO_DeinitMenu();
}
void TEMPO_Renew() {
    TEMPO_RenewMenu();
}
void TEMPO_Draw() {
    TEMPO_DrawMenu();
}