#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <stdlib.h>
#include <stdio.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string.h>

#include "../toml/toml.h"


#include "basic.h"
#include "debug.h"
#include "device.h"
#include "menu.h"


int main(int argc, char *argv[]) {
    // window & renderer
    const char WINDOW_TITLE[] = "Test";
    const int WINDOW_WIDTH = 800, WINDOW_HEIGHT = 600;
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

    // create & load
    loadDebugTheme();

    loadMenu(renderer);


    // running
    while (running) {
        // printf("                                        \n");
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {running = false;}
        }

        // physical renew
        renewScreenParas(window);
        renewMouse();

        // logical renew
        menu.bck_rect.w = (float)windowWidth, menu.bck_rect.h = (float)windowHeight;
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
