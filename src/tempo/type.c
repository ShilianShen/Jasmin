#include "type.h"


struct TEMPO_Type {
    int idx;
    void* data;
};


// CREATE & DELETE =====================================================================================================
TEMPO_Type* TEMPO_CreateType(const cJSON* type_json) {
    REQ_CONDITION(type_json != NULL, return NULL);

    TEMPO_Type* type = calloc(1, sizeof(TEMPO_Type));
    REQ_CONDITION(type != NULL, return NULL);

    char* type_name = NULL;
    REQ_CONDITION(cJSON_LoadByKey(type_json, "type", JSM_STRING, &type_name), return NULL);

    REQ_CONDITION(BASIC_GetTableIdxByKey(TEMPO_TYPE_FUNC_TABLE, type_name, &type->idx), return NULL);

    const TEMPO_TypeFunc2* typeFunc = BASIC_GetTableValByIdx(TEMPO_TYPE_FUNC_TABLE, type->idx);
    REQ_CONDITION(typeFunc != NULL, return NULL);

    type->data = typeFunc->create(type_json);
    REQ_CONDITION(type->data != NULL, return NULL);

    return type;
}
TEMPO_Type* TEMPO_DeleteType(TEMPO_Type* type) {
    if (type != NULL) {
        if (type->data != NULL) {
            const TEMPO_TypeFunc2* typeFunc = BASIC_GetTableValByIdx(TEMPO_TYPE_FUNC_TABLE, type->idx);
            REQ_CONDITION(typeFunc != NULL, return NULL);
            typeFunc->delete(type->data);
        }
        free(type);
        type = NULL;
    }
    return NULL;
}


// RENEW ===============================================================================================================
SDL_Texture* TEMPO_RenewTypeTexture(TEMPO_Type* type) {
    REQ_CONDITION(type != NULL, return false);

    const TEMPO_TypeFunc2* typeFunc = BASIC_GetTableValByIdx(TEMPO_TYPE_FUNC_TABLE, type->idx);
    REQ_CONDITION(typeFunc != NULL, return false);

    REQ_CONDITION(typeFunc->texture != NULL, return false);
    return typeFunc->texture(type->data);
}
bool TEMPO_RenewTypeTrig(TEMPO_Type* type, const SDL_FRect dst_rect) {
    REQ_CONDITION(type != NULL, return false);

    const TEMPO_TypeFunc2* typeFunc = BASIC_GetTableValByIdx(TEMPO_TYPE_FUNC_TABLE, type->idx);
    REQ_CONDITION(typeFunc != NULL, return false);

    REQ_CONDITION(typeFunc->trig != NULL, return false);
    typeFunc->trig(&type->data, dst_rect);
    return true;
}



