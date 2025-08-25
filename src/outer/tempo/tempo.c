#include "menu.h"
#include "tempo.h"


bool TEMPO_Load() {
    TEMPO_LoadTheme();
    TEMPO_LoadMenu();
    return true;
}
void TEMPO_Unload() {
    TEMPO_UnloadMenu();
    TEMPO_UnloadTheme();
}
void TEMPO_Renew() {
    if (TEMPO_OFEN_RELOAD) {
        DEBUG_SendMessageL("reload = true\n");
        TEMPO_Unload();
        TEMPO_Load();
    }
    TEMPO_RenewMenu();
}


void TEMPO_Draw() {
    TEMPO_DrawMenu();
}