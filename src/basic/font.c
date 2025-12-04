#include "font.h"

#include "func.h"


static Table BASIC_FontSet;


TTF_Font* BASIC_GetFont(const char* key) {
    REQ_CONDITION(key != NULL, return false);
    return BASIC_GetTableValByKey(BASIC_FontSet, key);
}
static void* BASIC_CreateFont(const cJSON* font_json) {
    REQ_CONDITION(font_json != NULL, return NULL);
    char* path; REQ_CONDITION(cJSON_LoadByKey(font_json, "path", JSM_STRING, &path), return false);
    float size; REQ_CONDITION(cJSON_LoadByKey(font_json, "size", JSM_FLOAT, &size), return false);
    TTF_Font* font = TTF_OpenFont(path, size);
    REQ_CONDITION(font != NULL, return NULL);
    return font;
}
static void* BASIC_DeleteFont(void* font_void) {
    OPT_CONDITION(font_void != NULL, return font_void);
    TTF_Font* font = font_void;
    TTF_CloseFont(font);
    return NULL;
}


bool BASIC_InitFont() {
    const cJSON* fontSet_json = cJSON_GetObjectItem(basic_json, "fontSet");
    REQ_CONDITION(fontSet_json != NULL, return false);
    BASIC_CreateTable(&BASIC_FontSet, fontSet_json, BASIC_CreateFont);
    return true;
}
void BASIC_ExitFont() {
    BASIC_DeleteTable(&BASIC_FontSet, BASIC_DeleteFont);
}