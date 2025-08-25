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


static void INIT() {
    BASIC_Init();
    DEBUG_Init(renderer);
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
        const char* myString = strdup("123");
        //
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_EVENT_QUIT: running = false; break;
                default: break;
            }
        }
        RENEW();
        DEBUG_SendMessageR("%d, %f, %d\n", test_int, test_float, test_bool);
        DRAW();
        // SDL_GPUShader* shader = SDL_CreateGPUShader();
    }
    UNLOAD();
    DEINIT();
}