#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "debug.h"
#include "device.h"
#include "menu.h"


int main(int argc, char *argv[]) {
    const char WINDOW_TITLE[] = "Test";
    const int WINDOW_WIDTH = 800;
    const int WINDOW_HEIGHT = 600;
    SDL_Window *window;
    SDL_Renderer *renderer;
    bool running = true;
    const SDL_WindowFlags FLAG = SDL_WINDOW_OPENGL | SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_RESIZABLE;
    SDL_Event event;

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
    }
    if (!TTF_Init()) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize TTF: %s", SDL_GetError());
    }
    if (!SDL_CreateWindowAndRenderer(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, FLAG, &window, &renderer)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize window and render: %s", SDL_GetError());
    }

    loadDebug();
    loadMenuTheme();
    renewScreenScale(window);

    Elem elemRoot;
    SDL_FRect guide = {0, 0, 0, 0};
    loadElemOthers(&elemRoot, 40, guide, 0, 0);
    loadElemTexture(renderer, &elemRoot, "t-FUCK WORLD!");

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {running = false;}
        }

        renewMouse();
        bck_rect.w = WINDOW_WIDTH * scale_x;
        bck_rect.h = WINDOW_HEIGHT * scale_y;
        renewElem(&elemRoot);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        drawElem(renderer, &elemRoot);

        drawMouse(renderer);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

}
