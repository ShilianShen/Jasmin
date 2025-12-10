#include "manu.h"


bool createManu(void* info, const cJSON* info_json) {
    TEMPO_TypeManu** manu2 = info;
    REQ_CONDITION(cJSON_IsObject(info_json), return false);

    char* key = NULL;
    REQ_CONDITION(cJSON_LoadByKey(info_json, "key", JSM_STRING, &key), return false);

    *manu2 = BASIC_GetTableValByKey(TEMPO_PTR_TABLE, key);
    REQ_CONDITION(*manu2 != NULL, return false);

    return true;
}
void deleteManu(void* info) {

}
SDL_Texture* textureManu(void* info) {
    const TEMPO_TypeManu* manu = *(TEMPO_TypeManu**)info;
    return manu->texture != NULL ? manu->texture(manu->info) : NULL;
}
bool trigManu(void* info, const SDL_FPoint mouse, const SDL_FRect dst_rect) {
    const TEMPO_TypeManu* manu = *(TEMPO_TypeManu**)info;
    return manu->trig != NULL ? manu->trig(manu->info, dst_rect) : false;
}
