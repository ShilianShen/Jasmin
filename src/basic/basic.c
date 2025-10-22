#include "basic.h"


SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_FRect windowRectScaled = {0, 0, 0, 0};
SDL_FRect windowRect = {0, 0, 0, 0};


const char* WINDOW_TITLE = "Test";
const int WINDOW_WIDTH = 1200, WINDOW_HEIGHT = 800;
bool running = true;
const SDL_WindowFlags FLAG = SDL_WINDOW_METAL | SDL_WINDOW_HIGH_PIXEL_DENSITY;


int logical_w, logical_h;
int windowWidth, windowHeight;
float scale_x = 1, scale_y = 1;
SDL_Color EMPTY = {0, 0, 0, 0}, BLACK = {0, 0, 0, 255}, WHITE = {255, 255, 255, 255};
float BASIC_T1 = 0, BASIC_T2 = 0;


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

    BASIC_T2 = (float)SDL_GetTicks() / 1000;
    return true;
}
bool BASIC_Renew() {
    BASIC_T1 = BASIC_T2;
    BASIC_T2 = (float)SDL_GetTicks() / 1000;

    SDL_GetWindowSize(window, &logical_w, &logical_h);
    SDL_GetWindowSizeInPixels(window, &windowWidth, &windowHeight);
    scale_x = (float)windowWidth / (float)logical_w;
    scale_y = (float)windowHeight / (float)logical_h;
    windowRectScaled = (SDL_FRect){0, 0, (float)windowWidth * scale_x, (float)windowHeight * scale_y};
    windowRect = (SDL_FRect){0, 0, (float)windowWidth, (float)windowHeight};
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

