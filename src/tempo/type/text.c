#include "text.h"


bool TEMPO_CreateTypeText(void* info, const cJSON* info_json)   {
    TypeText* text = info;
    if (cJSON_IsObject(info_json) == false) {
        return false;
    }
    char* string_json = NULL;
    char* font_json = NULL;
    bool key_json = false;
    REQ_CONDITION(cJSON_LoadByKey(info_json, "string", JSM_STRING, &string_json), return false);
    REQ_CONDITION(cJSON_LoadByKey(info_json, "font", JSM_STRING, &font_json), return false);
    cJSON_LoadByKey(info_json, "key", JSM_BOOL, &key_json);

    cJSON_LoadByKey(info_json, "back_color", JSM_COLOR, &text->backColor);
    text->font = BASIC_GetFont(font_json);
    text->string = strdup(string_json);
    text->key = key_json;
    if (text->font == NULL || text->string == NULL) return false;

    const char* func_json = NULL; if (cJSON_LoadByKey(info_json, "func", JSM_STRING, &func_json)) {
        text->func = BASIC_GetTableValByKey(TEMPO_TrigFuncTable, func_json);
        REQ_CONDITION(text->func != NULL, return false);
    }
    const char* para_json = NULL; if (cJSON_LoadByKey(info_json, "para", JSM_STRING, &para_json)) {
        text->para_string = strdup(para_json);
        REQ_CONDITION(text->para_string != NULL, return false);
    }

    return true;
}
bool TEMPO_RenewTypeText(void *info, SDL_Texture** tex, SDL_FPoint mouse) {
    const TypeText* text = info;
    const char* string = text->string;
    if (text->key == true) {
        string = BASIC_GetTableValByKey(TEMPO_PTR_TABLE, text->string);
        REQ_CONDITION(string != NULL, return false);
    }
    if (*tex == NULL) {
        *tex = TXT_LoadTextureWithLines(
                renderer,
                text->font,
                string,
                WHITE,
                text->backColor,
                'C'
                );
    }
    REQ_CONDITION(*tex != NULL, return false);
    SDL_SetTextureScaleMode(*tex, SDL_SCALEMODE_NEAREST);
    if (&mouse != NULL && SDL_GetPointInTexture(mouse, *tex)) {
        PERPH_SetMouseKeyTrig(PERPH_MOUSE_KEY_LEFT, (Trig){text->func, (TrigPara)text->para_string, false});
    }
    return true;
}
void TEMPO_DeleteTypeText(void* info) {
    TypeText* text = info;
    if (text->string != NULL) {
        free(text->string);
        text->string = NULL;
    }
}