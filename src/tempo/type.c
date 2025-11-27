#include "type.h"



const TypeFunc TYPE_INFO_DETAIL[TEMPO_NUM_TYPES] = {
    [TEMPO_TYPE_FILE] = {"FILE", TEMPO_CreateTypeFile, TEMPO_RenewTypeFile, TEMPO_DeleteTypeFile, 0},
    [TEMPO_TYPE_TEXT] = {"TEXT", TEMPO_CreateTypeText, TEMPO_RenewTypeText, TEMPO_DeleteTypeText, 0},
    [TEMPO_TYPE_SLID] = {"SLID", TEMPO_CreateTypeSlid, TEMPO_RenewTypeSlid, NULL, {TEMPO_TrigFuncSlid, 0, true}},
    [TEMPO_TYPE_BOOL] = {"BOOL", TEMPO_CreateTypeBool, TEMPO_RenewTypeBool, NULL, {TEMPO_TrigFuncBool, 0, false}},
    [TEMPO_TYPE_MANU] = {"MANU", TEMPO_CreateTypeManu, TEMPO_RenewTypeManu, TEMPO_DeleteTypeManu, 0},
};
bool TEMPO_GetTypeIdFromString(TypeId* id, const char* string) {
    REQ_CONDITION(id != NULL, return false);
    REQ_CONDITION(string != NULL, return false);
    for (int i = 0; i < TEMPO_NUM_TYPES; i++) {
        if (strcmp(string, TYPE_INFO_DETAIL[i].name) == 0) {
            *id = i;
            return true;
        }
    }
    return false;
}


// GET & SET ===========================================================================================================
Trig TEMPO_GetTypeTrig(const TEMPO_Type* type) {
    REQ_CONDITION(type != NULL, return (Trig){0});
    return TYPE_INFO_DETAIL[type->id].trig;
}
SDL_Texture* TEMPO_GetTypeTexture(const TEMPO_Type* type) {
    REQ_CONDITION(type != NULL, return NULL);
    return type->texture;
}


// CREATE & DELETE =====================================================================================================
TEMPO_Type* TEMPO_CreateType(const cJSON* type_json) {
    REQ_CONDITION(type_json != NULL, return NULL);
    TEMPO_Type* type = calloc(1, sizeof(TEMPO_Type));
    REQ_CONDITION(type != NULL, return NULL);
    TEMPO_GetTypeIdFromString(&type->id, type_json->string);
    REQ_CONDITION(TYPE_INFO_DETAIL[type->id].create(&type->info, type_json), return NULL);
    return type;
}
TEMPO_Type* TEMPO_DeleteType(TEMPO_Type* type) {
    if (TYPE_INFO_DETAIL[type->id].delete != NULL) {
        TYPE_INFO_DETAIL[type->id].delete(&type->info);
    }
    SDL_DestroyTexture(type->texture);
    free(type);
    return NULL;
}


// RENEW ===============================================================================================================
bool TEMPO_RenewType(TEMPO_Type* type) {
    REQ_CONDITION(type != NULL, return false);
    TYPE_INFO_DETAIL[type->id].renew(&type->info, &type->texture);
    return true;
}