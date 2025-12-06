#include "type.h"
#include "type/text.h"
#include "type/file.h"
#include "type/slid.h"
#include "type/bool.h"
#include "type/manu.h"


struct TEMPO_Type {
    union {
        TypeFile file;
        TypeText text;
        TypeSlid slid;
        TypeBool bool_;
        TypeManu manu;
    } info;
    TypeId id;
    SDL_Texture* texture;
};
typedef struct {
    bool (*create)(void*, const cJSON*);
    bool (*renew)(void*, SDL_Texture**, SDL_FPoint);
    void (*delete)(void*);
} TypeFunc;
KeyVal typeKeyVal[TEMPO_NUM_TYPES] = {
    {"FILE", &(TypeFunc){TEMPO_CreateTypeFile, TEMPO_RenewTypeFile, TEMPO_DeleteTypeFile}},
    {"TEXT", &(TypeFunc){TEMPO_CreateTypeText, TEMPO_RenewTypeText, TEMPO_DeleteTypeText}},
    {"SLID", &(TypeFunc){TEMPO_CreateTypeSlid, TEMPO_RenewTypeSlid, TEMPO_DeleteTypeSlid}},
    {"BOOL", &(TypeFunc){TEMPO_CreateTypeBool, TEMPO_RenewTypeBool, TEMPO_DeleteTypeBool}},
    {"MANU", &(TypeFunc){TEMPO_CreateTypeManu, TEMPO_RenewTypeManu, TEMPO_DeleteTypeManu}}
};
Table typeFuncTable = {.kv = typeKeyVal, .len = len_of(typeKeyVal)};


// GET & SET ===========================================================================================================
Trig TEMPO_GetTypeTrig(const TEMPO_Type* type) {
    REQ_CONDITION(type != NULL, return (Trig){0});
    // return typeFuncTable.kv[type->id].trig;
    return (Trig){0};
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
    REQ_CONDITION(BASIC_GetTableIdxByKey(typeFuncTable, type_json->string, (int*)&type->id), return NULL);
    const TypeFunc* typeFunc = BASIC_GetTableValByIdx(typeFuncTable, type->id);
    REQ_CONDITION(typeFunc != NULL, return NULL);
    REQ_CONDITION(typeFunc->create(&type->info, type_json), return NULL);

    return type;
}
TEMPO_Type* TEMPO_DeleteType(TEMPO_Type* type) {
    const TypeFunc* typeFunc = BASIC_GetTableValByIdx(typeFuncTable, type->id);
    REQ_CONDITION(typeFunc != NULL, return NULL);
    if (typeFunc->delete != NULL) typeFunc->delete(&type->info);

    SDL_DestroyTexture(type->texture);
    free(type);
    return NULL;
}


// RENEW ===============================================================================================================
bool TEMPO_RenewTypeTrig(TEMPO_Type* type, const SDL_FPoint mouse) {
    REQ_CONDITION(type != NULL, return false);
    const TypeFunc* typeFunc = BASIC_GetTableValByIdx(typeFuncTable, type->id);
    REQ_CONDITION(typeFunc != NULL, return false);
    REQ_CONDITION(typeFunc->renew != NULL, return false);
    REQ_CONDITION(typeFunc->renew(&type->info, &type->texture, mouse), return false);
    return true;
}