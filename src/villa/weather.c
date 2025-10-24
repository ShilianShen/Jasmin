#include "weather.h"




const int NUM_RAIN = 2000;
SDL_FRect rainDrops[NUM_RAIN] = {0};
SDL_FPoint rainPos[NUM_RAIN] = {0};
typedef struct RainTheme {
    int num;
    float len;
    float arg;
    SDL_Color color;
    float min, max;
    float* arr;
} RainTheme;
RainTheme rainTheme = {
    .num = 1000,
    .len = 50,
    .arg = 0.0f,
    .color = {250, 250, 250, 128},
    .min = 1,
    .max = 4,
};


bool VILLA_InitRain() {
    for (int i = 0; i < NUM_RAIN; i++) {
        rainDrops[i].x = SDL_randf();
        rainDrops[i].y = SDL_randf();
        rainDrops[i].w = SDL_randf();
        rainDrops[i].h = SDL_randf();
    }
    return true;
}
bool VILLA_RenewRain() {
    const float cos = SDL_cosf(rainTheme.arg), sin = SDL_sinf(rainTheme.arg);
    const float A = (float)SDL_GetTicks();
    const float B = SDL_max(windowRect.w, windowRect.h);
    for (int i = 0; i < NUM_RAIN; i++) {
        const float rate = rainTheme.min + (rainTheme.max - rainTheme.min) * BASIC_AtvLinear(rainDrops[i].w);
        const float offset = SDL_fmodf(A * rate, 2 * B) - B;
        rainPos[i].x = rainDrops[i].x * windowRect.w + offset * sin;
        rainPos[i].y = rainDrops[i].y * windowRect.h + offset * cos;
    }
    return true;
}
bool VILLA_DrawRain() {
    const float dx = rainTheme.len * SDL_sinf(rainTheme.arg);
    const float dy = rainTheme.len * SDL_cosf(rainTheme.arg);
    SDL_SetRenderColor(renderer, rainTheme.color);
    for (int i = 0; i < NUM_RAIN; i++) {
        SDL_RenderLine(renderer,
            rainPos[i].x, rainPos[i].y,
            rainPos[i].x + dx, rainPos[i].y + dy
            );
    }
    return true;
}
void VILLA_ExitRain() {}