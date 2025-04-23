#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
// #include <SDL_gfx.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "debug.h"
#include "device.h"
#include "menu.h"


SDL_Window* window;
SDL_Renderer* renderer;
const char WINDOW_TITLE[] = "Menu Editor";


int main(int argc, char* argv[]) {
    // 初始化依赖库
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init 失败: %s\n", SDL_GetError());
        return 1;
    }
    if (TTF_Init() == -1) {
        SDL_Log("TTF_Init Error: %s", TTF_GetError());
        return 1;
    }

    // 创建 window
    window = SDL_CreateWindow(WINDOW_TITLE, 0, 0, 800, 600, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    if (!window) {
        printf("Fail SDL_CreateWindow: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // 创建 renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    renewScreenScale(window);


    // 载入
    loadMenuTheme();
    loadDebug();
    Elem elemRoot = {20, {100, 100, 100, 100}, {}};
    loadElemTexture(renderer, &elemRoot, "t-Root");
    SDL_Event event;
    bck_rect.w = 640;
    bck_rect.h = 480;

    // 主循环
    int running = 1;
    while (running) {
        // event
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(renderer, NULL);

        // renew
        renewMouse();
        renewElem(&elemRoot);

        // draw
        SDL_GetWindowSize(window, &bck_rect.w, &bck_rect.h);
        bck_rect.w *= scale_x;
        bck_rect.h *= scale_y;

        DEBUG_DrawRect(renderer, &bck_rect);
        drawElem(renderer, &elemRoot);
        drawMouse(renderer);
        SDL_RenderPresent(renderer);
    }

    // 释放
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}