#include "type.h"



const TypeFunc TYPE_INFO_DETAIL[TEMPO_NUM_TYPES] = {
    [TEMPO_TYPE_NULL] = {"NULL", NULL, NULL, NULL, 0},
    [TEMPO_TYPE_FILE] = {"FILE", TEMPO_CreateTypeFile, TEMPO_RenewTypeFile, TEMPO_DeleteTypeFile, 0},
    [TEMPO_TYPE_TEXT] = {"TEXT", TEMPO_CreateTypeText, TEMPO_RenewTypeText, TEMPO_DeleteTypeText, 0},
    [TEMPO_TYPE_SLID] = {"SLID", TEMPO_CreateTypeSlid, TEMPO_RenewTypeSlid, NULL, {TEMPO_TrigFuncSlid, 0, true}},
    [TEMPO_TYPE_BOOL] = {"BOOL", TEMPO_CreateTypeBool, TEMPO_RenewTypeBool, NULL, {TEMPO_TrigFuncBool, 0, false}},
};
TypeId TEMPO_GetTypeFromString(const char* string) {
    for (int i = 0; i < TEMPO_NUM_TYPES; i++) {
        if (strcmp(string, TYPE_INFO_DETAIL[i].name) == 0) {
            return i;
        }
    }
    return TEMPO_TYPE_NULL;
}
// GET & SET ===========================================================================================================
Trig TEMPO_GetTypeTrig(const Type* type) {
    REQ_CONDITION(type != NULL, return (Trig){0});
    return TYPE_INFO_DETAIL[type->id].trig;
}
SDL_Texture* TEMPO_GetTypeTexture(const Type* type) {
    REQ_CONDITION(type != NULL, return NULL);
    return type->texture;
}


// CREATE & DELETE =====================================================================================================
Type* TEMPO_CreateType(const cJSON* type_json) {
    REQ_CONDITION(type_json != NULL, return NULL);
    Type* type = calloc(1, sizeof(Type));
    REQ_CONDITION(type != NULL, return NULL);
    type->id = TEMPO_GetTypeFromString(type_json->string);
    REQ_CONDITION(type->id != TEMPO_TYPE_NULL, return NULL);
    REQ_CONDITION(TYPE_INFO_DETAIL[type->id].create(&type->info, type_json), return NULL);
    return type;
}
Type* TEMPO_DeleteType(Type* type) {
    if (TYPE_INFO_DETAIL[type->id].delete != NULL) {
        TYPE_INFO_DETAIL[type->id].delete(&type->info);
    }
    SDL_DestroyTexture(type->texture);
    free(type);
    return NULL;
}


// RENEW ===============================================================================================================
bool TEMPO_RenewType(Type* type) {
    REQ_CONDITION(type != NULL, return false);
    if (type->texture != NULL) {
        SDL_DestroyTexture(type->texture);
        type->texture = NULL;
    }
    TYPE_INFO_DETAIL[type->id].renew(&type->info, &type->texture);
    return true;
}