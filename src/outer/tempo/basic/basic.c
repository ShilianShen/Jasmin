#include "basic.h"


// BASIC ===============================================================================================================
Basic basic = (Basic){0};
static const char* BASIC_TOML = "../config/tempo_basic.toml";


// GET & SET ===========================================================================================================
void TEMPO_SetBasicRenderer(SDL_Renderer* renderer) {
    if (renderer == NULL) {
        printf("%s: renderer not exists.\n", __func__);
        return;
    } // Req Condition
    basic.renderer = basic.renderer = renderer;
}
void TEMPO_SetBasicFont(TTF_Font* font) {
    if (font == NULL) {
        printf("%s: font not exists.\n", __func__);
        return;
    } // Req Condition
    basic.font = font;
}
void TEMPO_SetBasicBckRect(const SDL_FRect bck_rect) {
    basic.bck = bck_rect;
}


// LOAD & UNLOAD =======================================================================================================
bool TEMPO_LoadBasic() {
    if (basic.renderer == NULL) {
        printf("%s: renderer not exists.\n", __func__);
        return false;
    } // Req Condition

    toml_table_t* tomlMenuTheme = getToml(BASIC_TOML); // alloc
    if (tomlMenuTheme == NULL) {
        printf("%s: failed from \"%s\".\n", __func__, BASIC_TOML);
        return false;
    } // Req Condition

    const toml_datum_t tomlFontPath = toml_string_in(tomlMenuTheme, "font_path");
    const toml_datum_t tomlFontSize = toml_double_in(tomlMenuTheme, "font_size");

    if (tomlFontPath.ok == true && tomlFontSize.ok == true) {
        basic.font = TTF_OpenFont(tomlFontPath.u.s, (float)tomlFontSize.u.d); // alloc
        if (basic.font == NULL) {
            printf("%s: failed from %s.\n", __func__, tomlFontPath.u.s);
        } // Req Condition
    }
    toml_free(tomlMenuTheme); // free
    return tomlFontPath.ok == true && tomlFontSize.ok == true && basic.font != NULL;
}
void TEMPO_UnloadBasic() {
    if (basic.font != NULL) {
        TTF_CloseFont(basic.font); // free
        basic.font = NULL;
    } // Opt Condition
}