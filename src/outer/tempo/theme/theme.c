#include "theme.h"


// BASIC ===============================================================================================================
Theme theme = (Theme){0};


// LOAD & UNLOAD =======================================================================================================
bool TEMPO_LoadTheme() {
    memset(&theme, 0, sizeof(theme));
    toml_table_t* tomlMenuTheme = getToml(TEMPO_THEME_TOML); // alloc
    if (tomlMenuTheme == NULL) {
        printf("%s: failed from \"%s\".\n", __func__, TEMPO_THEME_TOML);
        return false;
    } // Req Condition

    const toml_datum_t tomlFontPath = toml_string_in(tomlMenuTheme, "font_path");
    const toml_datum_t tomlFontSize = toml_double_in(tomlMenuTheme, "font_size");

    if (tomlFontPath.ok == true && tomlFontSize.ok == true) {
        theme.font = TTF_OpenFont(tomlFontPath.u.s, (float)tomlFontSize.u.d); // alloc
        if (theme.font == NULL) {
            printf("%s: failed from %s.\n", __func__, tomlFontPath.u.s);
        } // Req Condition
    }
    toml_free(tomlMenuTheme); // free
    return tomlFontPath.ok == true && tomlFontSize.ok == true && theme.font != NULL;
}
void TEMPO_UnloadTheme() {
    if (theme.font != NULL) {
        TTF_CloseFont(theme.font); // free
        theme.font = NULL;
    } // Opt Condition
}
