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


// CREATE & DELETE =====================================================================================================
Type* TEMPO_CreateType() {
    return NULL;
}
Type* TEMPO_DeleteType() {
    return NULL;
}


// RENEW ===============================================================================================================
bool TEMPO_RenewType(Type* type) {
    REQ_CONDITION(type != NULL, return false);
    TYPE_INFO_DETAIL[type->id].renew(&type->info, &type->texture);
    return true;
}