#include "basic.h"


SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_FRect windowRect = {0, 0, 0, 0};


const char* WINDOW_TITLE = "Test";
const int WINDOW_WIDTH = 1200, WINDOW_HEIGHT = 800;
bool running = true;
const SDL_WindowFlags FLAG = SDL_WINDOW_METAL | SDL_WINDOW_HIGH_PIXEL_DENSITY;


int logical_w, logical_h;
int windowWidth, windowHeight;
float scale_x = 1, scale_y = 1;
SDL_Color EMPTY = {0, 0, 0, 0};


ma_engine engine;


bool BASIC_Init() {
    const ma_result result = ma_engine_init(NULL, &engine);
    REQ_CONDITION(result == MA_SUCCESS, return false);

    REQ_CONDITION(SDL_Init(SDL_INIT_VIDEO), return false);
    REQ_CONDITION(TTF_Init(), return false);

    window = SDL_CreateWindow(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, FLAG);
    REQ_CONDITION(window != NULL, return false);

    renderer = SDL_CreateRenderer(window, NULL);
    REQ_CONDITION(renderer != NULL, return false);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    return true;
}
bool BASIC_Renew() {
    SDL_GetWindowSize(window, &logical_w, &logical_h);
    SDL_GetWindowSizeInPixels(window, &windowWidth, &windowHeight);
    scale_x = (float)windowWidth / (float)logical_w;
    scale_y = (float)windowHeight / (float)logical_h;
    windowRect = (SDL_FRect){0, 0, (float)windowWidth * scale_x, (float)windowHeight * scale_y};
    return true;
}
bool BASIC_Draw() {
    return true;
}
void BASIC_Exit() {
    SDL_DestroyWindow(window);
    window = NULL;

    SDL_Quit();

    ma_engine_uninit(&engine);
}

