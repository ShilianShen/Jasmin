#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <stdio.h>
#include <stdarg.h>
#include "jasmin/jasmin.h"



int main(int argc, char *argv[]) {
    // window & renderer
    const char WINDOW_TITLE[] = "Test";
    const int WINDOW_WIDTH = 800, WINDOW_HEIGHT = 800;
    bool oftenReload = true;
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

    /*if (ma_engine_init(NULL, &maEngine) != MA_SUCCESS) {
        return -1;
    }*/
    // ma_engine_play_sound(&maEngine, "../sound effects/CD_case_C.mp3", NULL);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_HideCursor();

    // init
    DEBUG_Init(renderer);
    MENU_Init(renderer);

    // load
    DEBUG_Load();
    MENU_Load("../src/menu_pages.toml", "../src/menu_theme.toml");

    // running
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {running = false;}
        }
        // renew
        // physical renew
        renewScreenParas(window);
        renewMouse();
        DEBUG_Renew();

        // logical renew
        menu.bck_rect.w = (float)windowWidth;
        menu.bck_rect.h = (float)windowHeight;
        MENU_Renew();

        // draw
        SDL_SetRenderSDLColor(renderer, COLOR_CLEAR);
        SDL_RenderClear(renderer);

        // logical draw
        MENU_Draw();

        // physical draw
        drawMouse(renderer);
        DEBUG_Intro();

        //
        SDL_RenderPresent(renderer);

        //
        if (oftenReload) {
            MENU_Kill();
            MENU_Load("../src/menu_pages.toml", "../src/menu_theme.toml");
        }
    }

    // kill & destroy
    MENU_Kill();
    SDL_DestroyWindow(window);
    SDL_Quit();

}
