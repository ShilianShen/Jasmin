#include "file.h"



bool createFile(void* info, const cJSON* info_json) {
    TypeFile* file = info;

    char* path_json = NULL;
    REQ_CONDITION(cJSON_LoadByKey(info_json, "path", JSM_STRING, &path_json), return false);

    file->texture = IMG_LoadTexture(renderer, path_json);
    REQ_CONDITION(file->texture != NULL, return false);
    SDL_SetTextureScaleMode(file->texture, SDL_SCALEMODE_NEAREST);

    const char* func_json = NULL; if (cJSON_LoadByKey(info_json, "func", JSM_STRING, &func_json)) {
        file->func = BASIC_GetTableValByKey(TEMPO_TrigFuncTable, func_json);
        REQ_CONDITION(file->func != NULL, return false);
    }
    const char* para_json = NULL; if (cJSON_LoadByKey(info_json, "para", JSM_STRING, &para_json)) {
        file->para_string = strdup(para_json);
        REQ_CONDITION(file->para_string != NULL, return false);
    }

    return true;
}
SDL_Texture* textureFile(void *info) {
    const TypeFile* file = info;
    return file->texture;
}
bool trigFile(void *info, SDL_FPoint mouse, SDL_FRect dst_rect) {
    const TypeFile* file = info;
    if (PERPH_GetMouseAndKeyInRect(PERPH_MOUSE_KEY_LEFT, dst_rect)) {
        PERPH_SetMouseKeyTrig(PERPH_MOUSE_KEY_LEFT, (Trig){file->func, (TrigPara)file->para_string, false});
    }
    return true;
}
void deleteFile(void* info) {
    TypeFile* file = info;
    SDL_DestroyTexture(file->texture);
}