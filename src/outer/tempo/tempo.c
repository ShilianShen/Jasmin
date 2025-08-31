#include "menu.h"


bool TEMPO_Init() {
    return TEMPO_LoadTheme() && TEMPO_LoadMenu();
}
void TEMPO_Exit() {
    TEMPO_UnloadMenu();
    TEMPO_UnloadTheme();
}
bool TEMPO_Renew() {
    bool result = true;
    if (TEMPO_OFEN_RELOAD) {
        DEBUG_SendMessageL("reload = true\n");
        TEMPO_Exit();
        result = result && TEMPO_Init();
    }
    result = result && TEMPO_RenewMenu();
    return result;
}
bool TEMPO_Draw() {
    TEMPO_DrawMenu();
    return true;
}