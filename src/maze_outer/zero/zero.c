#include "zero.h"




void ZERO_Load() {}
void ZERO_Unload() {}
void ZERO_Renew() {}
void ZERO_Draw() {
    ROOM_Draw_BDI();


    const float start_x = 100, start_y = 100;
    const float wall_w = 20, wall_h = 20;
    SDL_FRect rect = {0, 0, wall_w, wall_h};
    const Room* block = &roomSet[0];
    for (int i = 0; i < block->w; i++) {
        for (int j = 0; j < block->h; j++) {
            const bool wall = block->wall[i][j];
            if (wall == true) {
                rect.x = start_x + (float)i * wall_w;
                rect.y = start_y + (float)j * wall_h;
                SDL_SetRenderDrawColor(mazeRenderer, 128, 128, 255, 255);
                SDL_RenderFillRect(mazeRenderer, &rect);
            }
        }
    }
}
// DRAW ================================================================================================================
