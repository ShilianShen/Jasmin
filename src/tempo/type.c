// #include "type.h"
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
};


typedef struct {
    bool (*create)(void*, const cJSON*);
    SDL_Texture* (*texture)(void*);
    bool (*trig)(void*, SDL_FPoint);
    void (*delete)(void*);
} TypeFunc;
KeyVal typeKeyVal[TEMPO_NUM_TYPES] = {
    [TEMPO_TYPE_FILE] = {"FILE", &(TypeFunc){createFile, textureFile, trigFile, deleteFile}},
    [TEMPO_TYPE_TEXT] = {"TEXT", &(TypeFunc){createText, textureText, trigText, deleteText}},
    [TEMPO_TYPE_SLID] = {"SLID", &(TypeFunc){createSlid, textureSlid, trigSlid, deleteSlid}},
    [TEMPO_TYPE_BOOL] = {"BOOL", &(TypeFunc){createBool, textureBool, trigBool, deleteBool}},
    [TEMPO_TYPE_MANU] = {"MANU", &(TypeFunc){createManu, textureManu, trigManu, deleteManu}}
};
Table typeFuncTable = {.kv = typeKeyVal, .len = len_of(typeKeyVal)};


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

    free(type);
    return NULL;
}


// RENEW ===============================================================================================================
SDL_Texture* TEMPO_RenewTypeTexture(TEMPO_Type* type) {
    REQ_CONDITION(type != NULL, return false);

    const TypeFunc* typeFunc = BASIC_GetTableValByIdx(typeFuncTable, type->id);
    REQ_CONDITION(typeFunc != NULL, return false);

    REQ_CONDITION(typeFunc->texture != NULL, return false);
    return typeFunc->texture(&type->info);
}
bool TEMPO_RenewTypeTrig(TEMPO_Type* type, const SDL_FPoint mouse) {
    REQ_CONDITION(type != NULL, return false);

    const TypeFunc* typeFunc = BASIC_GetTableValByIdx(typeFuncTable, type->id);
    REQ_CONDITION(typeFunc != NULL, return false);

    REQ_CONDITION(typeFunc->trig != NULL, return false);
    return typeFunc->trig(&type->info, mouse);
}