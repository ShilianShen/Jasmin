#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <stdio.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

#define MINIAUDIO_IMPLEMENTATION
#include "../third_party/miniaudio/miniaudio.h"
#include "../third_party/toml/toml.h"
ma_engine maEngine;


// Jasmine
#include "basic.h"
#include "debug.h"
#include "device.h"
#include "menu.h"


int main(int argc, char *argv[]) {
    // window & renderer
    const char WINDOW_TITLE[] = "Test";
    const int WINDOW_WIDTH = 800, WINDOW_HEIGHT = 800;
    SDL_Window *window;
    SDL_Renderer *renderer;
    bool running = true;
    const SDL_WindowFlags FLAG = SDL_WINDOW_OPENGL | SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_RESIZABLE;
    SDL_Event event;
    const SDL_Color COLOR_CLEAR = {0, 0, 0, 255};

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
    }
    if (!TTF_Init()) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize TTF: %s", SDL_GetError());
    }

    if (!SDL_CreateWindowAndRenderer(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, FLAG, &window, &renderer)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize window and render: %s", SDL_GetError());
    }

    if (ma_engine_init(NULL, &maEngine) != MA_SUCCESS) {
        return -1;
    }
    // ma_engine_play_sound(&maEngine, "../sound effects/CD_case_C.mp3", NULL);

    // create & load
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    loadDebugTheme();

    menu.renderer = renderer;
    initMenu();


    // running
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {running = false;}
        }

        // physical renew
        renewScreenParas(window);
        renewMouse();

        // logical renew
        menu.bck_rect.w = (float)windowWidth, menu.bck_rect.h = (float)windowHeight;
        killMenu();
        loadMenu();
        renewMenu();

        // clear
        SDL_SetRenderSDLColor(renderer, COLOR_CLEAR);
        SDL_RenderClear(renderer);

        // logical draw
        drawMenu();

        // physical draw
        drawMouse(renderer);
        DEBUG_Intro(renderer);

        // show
        SDL_RenderPresent(renderer);
    }

    // kill & destroy
    SDL_DestroyWindow(window);
    SDL_Quit();

}
