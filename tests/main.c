#include "../include/jasmin/jasmin.h"


int test_int = 10;
float test_float = 10.5f;
const KeyValI TEMPO_TABLE_I[] = {
    {"test_int", &test_int},
    {NULL, NULL}
};
const KeyValF TEMPO_TABLE_F[] = {
    {"test_float", &test_float},
    {NULL, NULL}
};


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
        DEBUG_SendMessageR("%d, %f\n", test_int, test_float);
        DRAW();
        // SDL_GPUShader* shader = SDL_CreateGPUShader();
    }
    UNLOAD();
    DEINIT();
}