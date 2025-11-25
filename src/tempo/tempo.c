#include "menu.h"


Table TEMPO_TrigFuncTable;

// INIT & EXIT =========================================================================================================
bool TEMPO_Init() {
    const int M = TEMPO_INNER_TRIG_FUNC_TABLE.len;
    const int N = TEMPO_OUTER_TRIG_FUNC_TABLE.len;
    TEMPO_TrigFuncTable.kv = calloc(M + N, sizeof(KeyVal));
    REQ_CONDITION(TEMPO_TrigFuncTable.kv != NULL, return false);
    for (int i = 0; i < M; i++) TEMPO_TrigFuncTable.kv[i  ] = TEMPO_INNER_TRIG_FUNC_TABLE.kv[i];
    for (int i = 0; i < N; i++) TEMPO_TrigFuncTable.kv[i+M] = TEMPO_OUTER_TRIG_FUNC_TABLE.kv[i];
    TEMPO_TrigFuncTable.len = M + N;
    return TEMPO_LoadMenu();
}
void TEMPO_Exit() {
    TEMPO_UnloadMenu();
    free(TEMPO_TrigFuncTable.kv);
}


// RENEW ===============================================================================================================
bool TEMPO_Renew() {
    REQ_CONDITION(TEMPO_RenewMenu(), return false);
    return true;
}


// DRAW ================================================================================================================
bool TEMPO_Draw() {
    TEMPO_DrawMenu(); //REQ_CONDITION(TEMPO_DrawMenu(), return false);
    return true;
}