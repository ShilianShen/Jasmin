#include "file.h"

bool TEMPO_CreateElemFile(void* info, const cJSON* info_json) {
    ElemFileInfo* file = info;
    const char* string_json = NULL;
    if (cJSON_IsString(info_json)) {
        string_json = info_json->valuestring;
    }
    if (string_json != NULL) {
        file->string = strdup(string_json);
        if (file->string == NULL) {
            // printf("%s: failed in %s.\n", __func__, key);
            return false;
        }
    } // Req Condition
    return true;
}
bool TEMPO_RenewElemFile(const void* info, SDL_Texture** tex) {
    const ElemFileInfo* file = info;
    *tex = IMG_LoadTexture(renderer, file->string);
    if (*tex == NULL) {
        printf("%s: failed from \"%s\".\n", __func__, file->string);
        return false;
    } // Req Condition
    SDL_SetTextureScaleMode(*tex, SDL_SCALEMODE_NEAREST);
    return true;
}
void TEMPO_DeleteElemFile(void* info) {
    ElemFileInfo* file = info;
    if (file->string != NULL) {
        free(file->string);
        file->string = NULL;
    }
}