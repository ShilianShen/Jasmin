#include "theme.h"


// BASIC ===============================================================================================================
Theme theme = (Theme){0};


// LOAD & UNLOAD =======================================================================================================
static TTF_Font* TEMPO_LoadTheme_Font(const cJSON* font_json) {
    if (font_json == NULL) {
        return NULL;
    }
    const char* key = NULL;
    const char* path = NULL;
    float size = 0;
    if (cJSON_ExistKey(font_json, key = "path")) {
        if (cJSON_LoadFromObj(font_json, key, JSM_STRING, &path) == false) {
            printf("%s: cJSON_LoadFromObj failed.\n", __func__);
            return false;
        } // Req Condition
    }
    if (cJSON_ExistKey(font_json, key = "size")) {
        if (cJSON_LoadFromObj(font_json, key, JSM_FLOAT, &size) == false) {
            printf("%s: cJSON_LoadFromObj failed.\n", __func__);
            return false;
        } // Req Condition
    }
    if (path == NULL || size <= 0) {
        return NULL;
    }
    TTF_Font* font = TTF_OpenFont(path, size);
    if (font == NULL) {
        printf("%s: TTF_OpenFont failed.\n", __func__);
        return NULL;
    }
    return font;
}
static bool TEMPO_LoadTheme_RK(const cJSON* theme_json) {
    const char* key = NULL;
    if (cJSON_ExistKey(theme_json, key = "textFont")) {
        const cJSON* font_json = cJSON_GetObjectItem(theme_json, key);
        if (font_json == NULL) {
            printf("%s: cJSON_GetObjectItem failed.\n", __func__);
            return false;
        }
        theme.textFont = TEMPO_LoadTheme_Font(font_json);
        if (theme.textFont == NULL) {
            printf("%s: Theme.textFont failed.\n", __func__);
            return false;
        }
    }
    return true;
}
bool TEMPO_LoadTheme() {

#ifdef TEMPO_THEME_JSON
    cJSON* theme_json = getJson(TEMPO_THEME_JSON);
#else
    cJSON* theme_json = getJson(TEMPO_DEFAULT_THEME_JSON);
#endif

    memset(&theme, 0, sizeof(theme));
    if (theme_json == NULL) {
        printf("%s: getJson == NULL.\n", __func__);
        return false;
    } // Req Condition

    const bool rk = TEMPO_LoadTheme_RK(theme_json);
    cJSON_Delete(theme_json);

    if (rk == false) {
        printf("%s: TEMPO_LoadTheme_RK failed.\n", __func__);
        TEMPO_UnloadTheme();
        return false;
    }
    return true;
}
void TEMPO_UnloadTheme() {
    if (theme.textFont != NULL) {
        TTF_CloseFont(theme.textFont); // free
        theme.textFont = NULL;
    } // Opt Condition
}
