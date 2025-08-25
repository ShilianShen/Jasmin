#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>
#include "../include/jasmin/jasmin.h"


int test_int = 10;
float test_float = 10.5f;
bool test_bool = true;


const KeyVal TEMPO_TABLE_INT[] = {
    {"test_int", &test_int},
    {NULL, NULL}
};
const KeyVal TEMPO_TABLE_FLOAT[] = {
    {"test_float", &test_float},
    {NULL, NULL}
};
const KeyVal TEMPO_TABLE_BOOL[] = {
    {"test_bool", &test_bool},
    {NULL, NULL}
};


SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
    SDL_SetAppMetadata("SDL Hello World Example", "1.0", "com.example.sdl-hello-world");
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_HideCursor();

    BASIC_Init();
    DEBUG_Init(renderer);

    DEBUG_Load();
    TEMPO_Load();

    return SDL_APP_CONTINUE;
}
void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    TEMPO_Unload();
    BASIC_Deinit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
SDL_AppResult SDL_AppIterate(void *appstate) {
    renewScreenParas(window);
    DEVICE_RenewMouse();
    DEBUG_Renew();
    TEMPO_Renew();

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    TEMPO_Draw();

    DEVICE_DrawMouse(renderer);
    DEBUG_DrawMessage();

    SDL_RenderPresent(renderer);
    return SDL_APP_CONTINUE;
}
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
    switch (event->type) {
        case SDL_EVENT_QUIT: return SDL_APP_SUCCESS;
        default: break;
    }
    return SDL_APP_CONTINUE;
}