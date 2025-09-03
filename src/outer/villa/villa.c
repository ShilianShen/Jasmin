#include "villa.h"


const int NUM_RAIN = 100;
SDL_FPoint rain[NUM_RAIN] = {0};


bool VILLA_Init() {
    for (int i = 0; i < NUM_RAIN; i++) {
        rain[i].x = (float)WINDOW_WIDTH * SDL_randf();
        rain[i].y = (float)WINDOW_HEIGHT * SDL_randf();
        // printf("%f, %f\n", rain[i].x, rain[i].y);
    }
    return true;
}
bool VILLA_Renew() {

    return true;
}
bool VILLA_Draw() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 128);
    for (int i = 0; i < NUM_RAIN; i++) {
        SDL_FRect rect = {rain[i].x, rain[i].y, 40, 40};
        SDL_RenderFillRect(renderer, &rect);
    }
    return true;
}
void VILLA_Exit() {

}
