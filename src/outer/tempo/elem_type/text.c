#include "text.h"


bool TEMPO_CreateElemText(void* info, const cJSON* info_json)   {
    ElemTextInfo* text = info;
    if (cJSON_IsObject(info_json) == false) {
        return false;
    }
    const char* key = NULL;
    char* string_json = NULL;
    char* font_json = NULL;
    char* type_json = NULL;
    if (cJSON_LoadFromObj(info_json, key = "string", JSM_STRING, &string_json) == false) {
        printf("%s: failed in %s.\n", __func__, key);
        return false;
    }
    if (cJSON_LoadFromObj(info_json, key = "font", JSM_STRING, &font_json) == false) {
        printf("%s: failed in %s.\n", __func__, key);
        return false;
    }
    cJSON_LoadFromObj(info_json, key = "type", JSM_STRING, &type_json);
    if (type_json != NULL) {
        if (strcmp(type_json, "int") == 0) {
            text->type = JSM_INT;
        }
    }
    else {
        text->type = JSM_VOID;
    }
    text->font = TABLE_GetValByKey(theme.fontTable, font_json);
    text->string = strdup(string_json);
    if (text->font == NULL || text->string == NULL) {
        return false;
    }
    return true;
}
bool TEMPO_RenewElemText(const void* info, SDL_Texture** tex) {
    const ElemTextInfo* text = info;
    const char* string = text->string;
    char buffer[20];
    if (text->type == JSM_VOID) {
        const int* val = TABLE_GetValByKey(TEMPO_ExternTable[JSM_INT], text->string);
        if (val != NULL) {
            snprintf(buffer, sizeof(buffer), "%d", *val);
            string = buffer;
        }
    }
    *tex = TXT_LoadTextureWithLines(
                renderer,
                text->font,
                string,
                (SDL_Color){255, 255, 255, 255},
                EMPTY,
                'C'
                );
    if (*tex == NULL) {
        printf("%s: failed from \"%s\".\n", __func__, text->string);
        return false;
    } // Req Condition
    SDL_SetTextureScaleMode(*tex, SDL_SCALEMODE_NEAREST);
    return true;
}
void TEMPO_DeleteElemText(void* info) {
    ElemTextInfo* text = info;
    if (text->string != NULL) {
        free(text->string);
        text->string = NULL;
    }
}