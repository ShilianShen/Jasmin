#include "basic.h"


SDL_Window* window = NULL;
SDL_FRect windowRect = {0, 0, 0, 0};
SDL_FPoint windowScale = {1, 1};
const char* WINDOW_TITLE = "Test";
const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 800;


SDL_Renderer* renderer = NULL;
SDL_Event sdl_event;
bool running = true;
SDL_Color EMPTY = {0, 0, 0, 0}, BLACK = {0, 0, 0, 255}, WHITE = {255, 255, 255, 255};
float BASIC_DT = 0;
ma_engine engine;


bool BASIC_Init() {
    const ma_result result = ma_engine_init(NULL, &engine);
    REQ_CONDITION(result == MA_SUCCESS, return false);

    REQ_CONDITION(SDL_Init(SDL_INIT_VIDEO), return false);
    REQ_CONDITION(TTF_Init(), return false);

    const SDL_WindowFlags FLAGS = SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_RESIZABLE | SDL_WINDOW_METAL;
    window = SDL_CreateWindow(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, FLAGS);
    REQ_CONDITION(window != NULL, return false);

    renderer = SDL_CreateRenderer(window, NULL);
    REQ_CONDITION(renderer != NULL, return false);
    SDL_SetRenderVSync(renderer, true);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    return true;
}
void BASIC_Exit() {
    SDL_DestroyWindow(window); window = NULL;
    SDL_Quit();
    ma_engine_uninit(&engine);
}


bool BASIC_Renew() {
    static float BASIC_T1 = 0, BASIC_T2 = 0;
    BASIC_T1 = BASIC_T2;
    BASIC_T2 = (float)SDL_GetTicks() / 1000;
    BASIC_DT = BASIC_T2 - BASIC_T1;

    int logical_w, logical_h;
    SDL_GetWindowSize(window, &logical_w, &logical_h);

    int physical_w, windowHeight;
    SDL_GetWindowSizeInPixels(window, &physical_w, &windowHeight);

    windowScale = (SDL_FPoint){(float)physical_w / (float)logical_w, (float)windowHeight / (float)logical_h};
    windowRect = (SDL_FRect){0, 0, (float)physical_w, (float)windowHeight};

    return true;
}
bool BASIC_Draw() {
    return true;
}

