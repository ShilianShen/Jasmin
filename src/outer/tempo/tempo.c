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
    static bool reload = !true;
    if (reload == true) {
        DEBUG_SendMessageL("reload = true\n");
        TEMPO_Unload();
        TEMPO_Load();
    }
    TEMPO_RenewMenu();
}


void TEMPO_Draw() {
    TEMPO_DrawMenu();
}