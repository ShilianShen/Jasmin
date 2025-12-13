#include "lotri.h"


SDL_Texture* LOTRI_texture = NULL;


bool LOTRI_Init() {
    LOTRI_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 800, 600);
    REQ_CONDITION(LOTRI_texture != NULL, return false);

    SDL_SetTextureScaleMode(LOTRI_texture, SDL_SCALEMODE_NEAREST);
    return true;
}
bool LOTRI_Renew() {
    LOTRI_ClearBuffer();
    return true;
}
bool LOTRI_Draw() {
    LOTRI_RenewCamera();
    LOTRI_DrawCamera();
    LOTRI_DrawBuffer();
    return true;
}
void LOTRI_Exit() {
    SDL_DestroyTexture(LOTRI_texture);
}