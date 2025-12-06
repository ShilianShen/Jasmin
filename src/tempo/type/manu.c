#include "manu.h"


bool createManu(void* info, const cJSON* info_json) {
    TypeManu* manu = info;
    REQ_CONDITION(cJSON_IsObject(info_json), return false);
    char* key = NULL;
    REQ_CONDITION(cJSON_LoadByKey(info_json, "key", JSM_STRING, &key), return false);
    manu->info = BASIC_GetTableValByKey(TEMPO_PTR_TABLE, key);
    REQ_CONDITION(manu->info != NULL, return false);
    return true;
}
void deleteManu(void* info) {

}
SDL_Texture* textureManu(void* info) {
    const TypeManu* manu = info;
    return NULL;
}
bool trigManu(void* info, const SDL_FPoint mouse) {
    return true;
}
