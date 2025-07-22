#include <stdio.h>
#include <stdarg.h>
#include "jasmin/jasmin.h"
#include "maze/maze.h"


SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture* background;


const char WINDOW_TITLE[] = "Test";
const int WINDOW_WIDTH = 800, WINDOW_HEIGHT = 800;
bool oftenReload = false;
bool running = true;
const SDL_WindowFlags FLAG = SDL_WINDOW_OPENGL | SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_RESIZABLE;
SDL_Event event;


static void INIT() {
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

    // Jasmin
    DEBUG_Init(renderer);
    LOPO_Init();
    TEMPO_SetBasicRenderer(renderer);
    TEMPO_InitMenu();

    MAZE_Init(renderer);
    //LOTRI_Init(renderer);

    background = IMG_LoadTexture(renderer, "../images/Webb's_First_Deep_Field.jpg");
}
static void DEINIT() {
    // Jasmin
    TEMPO_DeinitMenu();
    LOPO_Deinit();
    //LOTRI_Deinit();

    MAZE_Deinit();

    // SDL
    SDL_DestroyTexture(background);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

static void LOAD() {
    DEBUG_Load();
    LOPO_Load();
    TEMPO_LoadMenu("../src/menu_pages.toml", "../src/menu_theme.toml");
    MAZE_Load();
}
static void UNLOAD() {
    LOPO_Unload();
    TEMPO_UnloadMenu();
    MAZE_Unload();
}

static void RENEW() {
    // physical renew
    renewScreenParas(window);
    renewMouse();
    DEBUG_Renew();

    //
    LOPO_Renew();
    MAZE_Renew();
    //LOTRI_Renew();

    // logical renew
    const SDL_FRect bck_rect = {0, 0, (float)windowWidth, (float)windowHeight};
    TEMPO_SetBasicBckRect(bck_rect);
    TEMPO_RenewMenu(bck_rect);
}
static void DRAW() {
    const static SDL_Color COLOR_CLEAR = {0, 0, 0, 255};
    SDL_SetRenderSDLColor(renderer, COLOR_CLEAR);
    SDL_RenderClear(renderer);

    // logical draw
    // SDL_RenderTextureAligned(renderer, background, NULL, NULL, NULL, 40);
    // TEST_Draw();
    MAZE_Draw();
    LOPO_Draw();
    //LOTRI_Draw();

    // TEMPO_DrawMenu();


    // physical draw
    drawMouse(renderer);
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
        DRAW();
        // SDL_GPUShader* shader = SDL_CreateGPUShader();
        if (oftenReload) {
            TEMPO_UnloadMenu();
            TEMPO_LoadMenu("../src/menu_pages.toml", "../src/menu_theme.toml");
        }
    }
    UNLOAD();
    DEINIT();
}