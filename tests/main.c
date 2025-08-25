#include "../include/jasmin/jasmin.h"



static bool Init() {
    BASIC_Init();
    DEBUG_Init(renderer);
    DEBUG_Load();
    TEMPO_Load();
    return true;
}
static bool Quit() {
    TEMPO_Unload();
    BASIC_Deinit();
    return true;
}
static bool Renew() {
    renewScreenParas(window);
    DEVICE_RenewMouse();
    DEBUG_Renew();
    TEMPO_Renew();
    return true;
}

static bool Draw() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    TEMPO_Draw();

    DEVICE_DrawMouse(renderer);
    DEBUG_DrawMessage();

    SDL_RenderPresent(renderer);
    return true;
}
int main() {
    Init();
    while (running) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_EVENT_QUIT: running = false; break;
                default: break;
            }
        }
        Renew();
        Draw();
    }
    Quit();
    return 0;
}
