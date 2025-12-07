#include "basic.h"


SDL_Window* window = NULL;
cJSON* basic_json = NULL;
SDL_FRect windowRect = {0, 0, 0, 0};
SDL_FPoint windowScale = {1, 1};
const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 800;
const size_t JSM_DataTypeSize[JSM_NUM_TYPES] = {
    [JSM_VOID] = sizeof(void),
    [JSM_PTR] = sizeof(void*),
    [JSM_INT] = sizeof(int),
    [JSM_FLOAT] = sizeof(float),
    [JSM_STRING] = sizeof(char*),
    [JSM_BOOL] = sizeof(bool),
    [JSM_FRECT] = sizeof(SDL_FRect),
    [JSM_RECT] = sizeof(SDL_Rect),
    [JSM_COLOR] = sizeof(SDL_Color),
};


SDL_Renderer* renderer = NULL;
SDL_Event sdl_event;
bool running = true;
float BASIC_T1 = 0, BASIC_T2 = 0, BASIC_DT = 0;
ma_engine engine;
SDL_Surface* iconSurface = NULL;


bool BASIC_Init() {
    const ma_result result = ma_engine_init(NULL, &engine);
    REQ_CONDITION(result == MA_SUCCESS, return false);

    REQ_CONDITION(SDL_Init(SDL_INIT_VIDEO), return false);
    REQ_CONDITION(TTF_Init(), return false);

    basic_json = getJson(BASIC_JSON);
    REQ_CONDITION(basic_json != NULL, return false);

    char* title_json = NULL;
    REQ_CONDITION(cJSON_LoadByKey(basic_json, "title", JSM_STRING, &title_json), return false);

    const SDL_WindowFlags FLAGS = SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL;
    window = SDL_CreateWindow(title_json, WINDOW_WIDTH, WINDOW_HEIGHT, FLAGS);
    REQ_CONDITION(window != NULL, return false);

    renderer = SDL_CreateRenderer(window, NULL);
    REQ_CONDITION(renderer != NULL, return false);
    SDL_SetRenderVSync(renderer, true);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    REQ_CONDITION(BASIC_InitFont(), return false);

    char* icon_json = NULL;
    REQ_CONDITION(cJSON_LoadByKey(basic_json, "icon", JSM_STRING, &icon_json), return false);

    iconSurface = IMG_Load(icon_json);
    REQ_CONDITION(iconSurface != NULL, return false);
    SDL_SetWindowIcon(window, iconSurface);

    return true;
}
void BASIC_Exit() {
    SDL_DestroyWindow(window); window = NULL;
    SDL_Quit();
    ma_engine_uninit(&engine);
    BASIC_ExitFont();
}


bool BASIC_Renew() {
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

