#include "manu.h"


bool TEMPO_CreateTypeManu(void* info, const cJSON* info_json) {
    TypeManu* manu = info;
    REQ_CONDITION(cJSON_IsObject(info_json), return false);
    char* key = NULL;
    REQ_CONDITION(cJSON_LoadByKey(info_json, "key", JSM_STRING, &key), return false);
    manu->info = BASIC_GetTableValByKey(TEMPO_PTR_TABLE, key);
    REQ_CONDITION(manu->info != NULL, return false);
    return true;
}
bool TEMPO_RenewTypeManu(void *info, SDL_Texture** texture, const SDL_FPoint* point) {
    const TypeManu* manu = info;
    if (manu->info->renew != NULL) {
        manu->info->renew(manu->info->info, texture, point);
    }
    return true;
}
void TEMPO_DeleteTypeManu(void* info) {

}