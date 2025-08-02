#include "basic.h"


//
SDL_Renderer* tempoRenderer;
TTF_Font* tempoTextFont;
SDL_FRect tempoBckRect = {0, 0, 0, 0};
static bool tempoOk = false;


//
static void TEMPO_CheckBasicOk() {
    tempoOk = false;
    if (tempoRenderer != NULL && tempoTextFont != NULL) {
        tempoOk = true;
    }
}


// GET & SET ===========================================================================================================
bool TEMPO_GetBasicOk() {
    return tempoOk;
}
void TEMPO_SetBasicRenderer(SDL_Renderer* renderer) {
    if (renderer == NULL) {
        printf("%s: renderer not exists.\n", __func__);
        return;
    } // Req Condition
    tempoRenderer = renderer;
    TEMPO_CheckBasicOk();
}
void TEMPO_SetBasicFont(TTF_Font* font) {
    if (font == NULL) {
        printf("%s: font not exists.\n", __func__);
        return;
    } // Req Condition
    tempoTextFont = font;
    TEMPO_CheckBasicOk();
}
void TEMPO_SetBasicBckRect(const SDL_FRect bck_rect) {
    tempoBckRect = bck_rect;
    TEMPO_CheckBasicOk();
}