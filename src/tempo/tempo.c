#include "menu.h"

Table TEMPO_TrigFuncTable;


bool TEMPO_Init() {
    TEMPO_TrigFuncTable.len = TEMPO_INNER_TRIG_TABLE.len + TEMPO_OUTER_TRIG_TABLE.len;
    TEMPO_TrigFuncTable.kv = calloc(TEMPO_TrigFuncTable.len, sizeof(KeyVal));
    REQ_CONDITION(TEMPO_TrigFuncTable.kv != NULL, return false);
    for (int i = 0; i < TEMPO_INNER_TRIG_TABLE.len; i++) {
        TEMPO_TrigFuncTable.kv[i] = TEMPO_INNER_TRIG_TABLE.kv[i];
    }
    for (int i = 0; i < TEMPO_OUTER_TRIG_TABLE.len; i++) {
        TEMPO_TrigFuncTable.kv[i+TEMPO_INNER_TRIG_TABLE.len] = TEMPO_INNER_TRIG_TABLE.kv[i];
    }
    return TEMPO_LoadTheme() && TEMPO_LoadMenu();
}
void TEMPO_Exit() {
    TEMPO_UnloadMenu();
    TEMPO_UnloadTheme();
    free(TEMPO_TrigFuncTable.kv);
}
bool TEMPO_Renew() {
    bool result = true;
    result = result && TEMPO_RenewMenu();
    return result;
}
bool TEMPO_Draw() {
    TEMPO_DrawMenu();
    return true;
}