#include "manu.h"


bool TEMPO_CreateTypeManu(void* info, const cJSON* info_json) {
    TypeManuInfo* manu = info;
    REQ_CONDITION(cJSON_IsObject(info_json), return false);
    char* key = NULL;
    REQ_CONDITION(cJSON_LoadByKey(info_json, "key", JSM_STRING, &key), return false);
    SDL_Texture** texture = BASIC_GetTableValByKey(TEMPO_PTR_TABLE, key);
    manu->texture = texture;
    return true;
}
bool TEMPO_RenewTypeManu(const void* info, SDL_Texture** tex) {
    const TypeManuInfo* manu = info;
    *tex = *manu->texture;
}
void TEMPO_DeleteTypeManu(void* info) {

}