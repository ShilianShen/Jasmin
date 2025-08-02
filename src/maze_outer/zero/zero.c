#include "zero.h"








// ZERO ================================================================================================================
void ZERO_Load() {
    ZERO_LoadRDI();
}
void ZERO_Unload() {
    ZERO_UnloadRDI();
}
void ZERO_Renew() {
    // ZERO_RenewRDI();
}
void ZERO_Draw() {
    // ZERO_DrawRDI();
    ZERO_DrawRoom();
    const SDL_FRect lifeBckRect = {100, 100, 1000, 100};
    const SDL_Color lifeBckColor = {64, 64, 64, 255};
    const SDL_FRect lifeRect = {100, 100, 10 * you->life, 100};
    const SDL_Color lifeColor = {64, 0, 0, 255};
    SDL_SetRenderSDLColor(mazeRenderer, lifeBckColor);
    SDL_RenderFillRect(mazeRenderer, &lifeBckRect);
    SDL_SetRenderSDLColor(mazeRenderer, lifeColor);
    SDL_RenderFillRect(mazeRenderer, &lifeRect);

}

