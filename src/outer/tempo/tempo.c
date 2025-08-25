#include "menu.h"
#include "tempo.h"


bool TEMPO_Init() {
    TEMPO_LoadTheme();
    TEMPO_LoadMenu();
    return true;
}
void TEMPO_Exit() {
    TEMPO_UnloadMenu();
    TEMPO_UnloadTheme();
}

bool TEMPO_Renew() {
    if (TEMPO_OFEN_RELOAD) {
        DEBUG_SendMessageL("reload = true\n");
        TEMPO_Exit();
        TEMPO_Init();
    }
    TEMPO_RenewMenu();
    return true;
}


bool TEMPO_Draw() {
    TEMPO_DrawMenu();
    return true;
}
