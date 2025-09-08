#include "setting.h"


SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_FRect windowRect = {0, 0, 0, 0};

const char* WINDOW_TITLE = "Test";
const int WINDOW_WIDTH = 800, WINDOW_HEIGHT = 600;
bool running = true;
const SDL_WindowFlags FLAG = SDL_WINDOW_OPENGL | SDL_WINDOW_HIGH_PIXEL_DENSITY; //  | SDL_WINDOW_RESIZABLE


int logical_w, logical_h;
int windowWidth, windowHeight;
float scale_x = 1, scale_y = 1;
SDL_Color EMPTY = {0, 0, 0, 0};


ma_result result;
ma_engine engine;


bool BASIC_Init() {
    // miniaudio
    result = ma_engine_init(NULL, &engine);
    if (result != MA_SUCCESS) {
        return false;
    }

    // SDL
    if (SDL_Init(SDL_INIT_VIDEO) == false) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
        return false;
    }
    if (TTF_Init() == false) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize TTF: %s", SDL_GetError());
        return false;
    }

    window = SDL_CreateWindow(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, FLAG);
    if (window == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize window and render: %s", SDL_GetError());
        return false;
    }
    renderer = SDL_CreateRenderer(window, NULL);
    if (renderer == NULL) {
        printf("渲染器创建失败: %s\n", SDL_GetError());
        return false;
    }
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_HideCursor();
    // SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    return true;
}
void BASIC_Exit() {
    // SDL
    SDL_DestroyWindow(window);
    SDL_Quit();
    // miniaudio
    ma_engine_uninit(&engine);
}

