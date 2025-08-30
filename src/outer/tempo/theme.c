#include "theme.h"


// BASIC ===============================================================================================================
Theme theme = (Theme){0};


// LOAD & UNLOAD =======================================================================================================
static bool TEMPO_LoadTheme_RK(const cJSON* theme_json) {
    const char* key = NULL;
    const char* font_path = NULL;
    float font_size = 0;
    SDL_Color font_color = {0};
    if (cJSON_ExistKey(theme_json, key = "font_path")) {
        if (cJSON_LoadFromObj(theme_json, key, JSM_STRING, &font_path) == false) {
            printf("%s: cJSON_LoadFromObj failed.\n", __func__);
            return false;
        } // Req Condition
    }
    if (cJSON_ExistKey(theme_json, key = "font_size")) {
        if (cJSON_LoadFromObj(theme_json, key, JSM_FLOAT, &font_size) == false) {
            printf("%s: cJSON_LoadFromObj failed.\n", __func__);
            return false;
        } // Req Condition
    }
    if (cJSON_ExistKey(theme_json, key = "font_color")) {
        if (cJSON_LoadFromObj(theme_json, key, JSM_COLOR, &font_color) == false) {
            printf("%s: cJSON_LoadFromObj failed.\n", __func__);
            return false;
        } // Req Condition
    }
    if (font_path != NULL && font_size > 0) {
        theme.font = TTF_OpenFont(font_path, font_size); // alloc
        if (theme.font == NULL) {
            printf("%s: failed from.\n", __func__);
        } // Req Condition
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
    if (theme.font != NULL) {
        TTF_CloseFont(theme.font); // free
        theme.font = NULL;
    } // Opt Condition
}
