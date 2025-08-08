#include "setting.h"



const char* WINDOW_TITLE = "Test";
const int WINDOW_WIDTH = 800, WINDOW_HEIGHT = 800;
bool oftenReload = false;
bool running = true;
const SDL_WindowFlags FLAG = SDL_WINDOW_OPENGL | SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_RESIZABLE;


int logical_w, logical_h;
int windowWidth, windowHeight;
float scale_x = 1, scale_y = 1;
SDL_Color EMPTY = {0, 0, 0, 0};



void BASIC_Init() {
    // SDL
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
    }
    if (!TTF_Init()) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize TTF: %s", SDL_GetError());
    }
    if (!SDL_CreateWindowAndRenderer(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, FLAG, &window, &renderer)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize window and render: %s", SDL_GetError());
    }
}
void BASIC_Deinit() {
    SDL_DestroyTexture(background);
    SDL_DestroyWindow(window);
    SDL_Quit();
}