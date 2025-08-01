#include "room.h"


void ZERO_DrawRoom() {
    int center = 0;
    SDL_Color wallColor = {0, 0, 0, 255};
    SDL_Color emptyColor = {255, 255, 255, 255};
    Room* room = &roomSet[center];
    float w = 50, h = 50;
    float x = 100, y = 100;
    for (int i = 0; i < room->w; i++) {
        for (int j = 0; j < room->h; j++) {
            SDL_FRect rect = {x + (float)i * w, y + (float)j * h, w, h};
            if (room->wall[i][j] == true) {
                SDL_SetRenderSDLColor(mazeRenderer, wallColor);
            }
            else {
                SDL_SetRenderSDLColor(mazeRenderer, emptyColor);
            }
            SDL_RenderFillRect(mazeRenderer, &rect);
        }
    }
}