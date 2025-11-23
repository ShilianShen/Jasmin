#include "text.h"


bool TEMPO_CreateTypeText(void* info, const cJSON* info_json)   {
    TypeTextInfo* text = info;
    if (cJSON_IsObject(info_json) == false) {
        return false;
    }
    const char* key = NULL;
    char* string_json = NULL;
    char* font_json = NULL;
    bool key_json = false;
    REQ_CONDITION(cJSON_LoadByKey(info_json, "string", JSM_STRING, &string_json), return false);
    REQ_CONDITION(cJSON_LoadByKey(info_json, "font", JSM_STRING, &font_json), return false);
    cJSON_LoadByKey(info_json, key = "key", JSM_BOOL, &key_json);

    text->font = BASIC_GetFont(font_json);
    text->string = strdup(string_json);
    text->key = key_json;
    if (text->font == NULL || text->string == NULL) {
        return false;
    }
    return true;
}
bool TEMPO_RenewTypeText(const void* info, SDL_Texture** tex) {
    const TypeTextInfo* text = info;
    const char* string = text->string;
    if (text->key == true) {
        string = BASIC_GetTableValByKey(TEMPO_ExternTable[JSM_STRING], text->string);
        REQ_CONDITION(string != NULL, return false);
    }
    *tex = TXT_LoadTextureWithLines(
                renderer,
                text->font,
                string,
                (SDL_Color){255, 255, 255, 255},
                EMPTY,
                'C'
                );
    REQ_CONDITION(*tex != NULL, return false);
    SDL_SetTextureScaleMode(*tex, SDL_SCALEMODE_NEAREST);
    return true;
}
void TEMPO_DeleteTypeText(void* info) {
    TypeTextInfo* text = info;
    if (text->string != NULL) {
        free(text->string);
        text->string = NULL;
    }
}