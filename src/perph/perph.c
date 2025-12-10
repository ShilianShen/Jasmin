#include "perph.h"


bool PERPH_Init() {
    cJSON* perph_json = getJson(PERPH_JSON);
    REQ_CONDITION(perph_json != NULL, return false);

    const cJSON* mouse_json = cJSON_GetObjectItem(perph_json, "mouse");
    REQ_CONDITION(PERPH_InitMouse(mouse_json), return false);

    cJSON_free(perph_json);
    return true;
}
void PERPH_Exit() {
    PERPH_ExitMouse();
}
bool PERPH_Renew() {
    REQ_CONDITION(PERPH_RenewMouse(), return false);
    REQ_CONDITION(PERPH_RenewBoard(), return false);
    return true;
}
bool PERPH_Draw() {
    REQ_CONDITION(PERPH_DrawMouse(), return false);
    REQ_CONDITION(PERPH_DrawBoard(), return false);
    return true;
}


