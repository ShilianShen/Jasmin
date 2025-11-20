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
    if (cJSON_ExistKey(font_json, key = "path")) {
        if (cJSON_LoadByKey(font_json, key, JSM_STRING, &path) == false) {
            printf("%s: cJSON_LoadByKey failed.\n", __func__);
            return false;
        } // Req Condition
    }

    float size = 0;
    if (cJSON_ExistKey(font_json, key = "size")) {
        if (cJSON_LoadByKey(font_json, key, JSM_FLOAT, &size) == false) {
            printf("%s: cJSON_LoadByKey failed.\n", __func__);
            return false;
        } // Req Condition
    }

    if (path == NULL || size <= 0) {
        return NULL;
    }
    TTF_Font* font = TTF_OpenFont(path, size); // alloc
    REQ_CONDITION(font != NULL, return NULL);

    return font;
}
static bool TEMPO_LoadTheme_RK(const cJSON* theme_json) {
    const char *key = NULL;
    if (cJSON_ExistKey(theme_json, key = "fontTable")) {
        const cJSON* fontTable_json = cJSON_GetObjectItem(theme_json, key);

        theme.fontTable.len = cJSON_GetArraySize(fontTable_json);
        if (theme.fontTable.len <= 0) {
            printf("%s: Theme.textFont failed.\n", __func__);
            return false;
        }

        theme.fontTable.kv = calloc(theme.fontTable.len, sizeof(KeyVal)); // alloc
        if (theme.fontTable.kv == NULL) {
            printf("%s: Theme.textFont failed.\n", __func__);
            return false;
        }

        for (int i = 0; i < theme.fontTable.len; i++) {
            const cJSON* kv_json = cJSON_GetArrayItem(fontTable_json, i);

            theme.fontTable.kv[i].key = strdup(kv_json->string); // alloc
            if (theme.fontTable.kv[i].key == NULL) {
                printf("%s: Theme.textFont failed.\n", __func__);
                return false;
            }

            const cJSON* font_json = cJSON_GetObjectItem(fontTable_json, theme.fontTable.kv[i].key);
            theme.fontTable.kv[i].val = TEMPO_LoadTheme_Font(font_json); // alloc
            if (theme.fontTable.kv[i].val == NULL) {
                printf("%s: Theme.textFont failed.\n", __func__);
                return false;
            }
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
    if (theme.fontTable.kv != NULL) {
        for (int i = 0; i < theme.fontTable.len; i++) {
            if (theme.fontTable.kv[i].val != NULL) {
                TTF_CloseFont(theme.fontTable.kv[i].val); // free
                theme.fontTable.kv[i].val = NULL;
            }
            if (theme.fontTable.kv[i].key != NULL) {
                free(theme.fontTable.kv[i].key); // free
                theme.fontTable.kv[i].key = NULL;
            }
        }
        free(theme.fontTable.kv); // free
    }

}
