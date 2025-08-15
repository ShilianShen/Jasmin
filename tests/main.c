#include "../include/jasmin/jasmin.h"


static void INIT() {
    BASIC_Init();
    DEBUG_Init(renderer);
    TEMPO_SetBasicRenderer(renderer);
    background = IMG_LoadTexture(renderer, "../images/Webb's_First_Deep_Field.jpg");
}
static void DEINIT() {
    BASIC_Deinit();
}

static void LOAD() {
    DEBUG_Load();
    TEMPO_Load();
}
static void UNLOAD() {
    TEMPO_Unload();
}

static void RENEW() {
    // physical renew
    renewScreenParas(window);
    DEVICE_RenewMouse();
    DEBUG_Renew();

    // logical renew
    const SDL_FRect bck_rect = {0, 0, (float)windowWidth, (float)windowHeight};
    TEMPO_SetBasicBckRect(bck_rect);

    TEMPO_Renew();
}
static void DRAW() {
    const static SDL_Color COLOR_CLEAR = {0, 0, 0, 255};
    SDL_SetRenderSDLColor(renderer, COLOR_CLEAR);
    SDL_RenderClear(renderer);
    TEMPO_Draw();


    // physical draw
    DEVICE_DrawMouse(renderer);
    DEBUG_DrawMessage();
    DEBUG_Intro();

    //
    SDL_RenderPresent(renderer);
    static bool reload = true;
    if (reload == true) {
        TEMPO_Unload();
        TEMPO_Load();
    }
}


int main(int argc, char *argv[]) {
    // window & renderer
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_HideCursor();

    //
    INIT();
    LOAD();
    while (running) {
        //
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }
        RENEW();
        DRAW();
        // SDL_GPUShader* shader = SDL_CreateGPUShader();
    }
    UNLOAD();
    DEINIT();
}