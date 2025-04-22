#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <SDL_ttf.h>
#include <SDL_rect.h>
#include <SDL_render.h>
#include <stdint.h>

#include "menu.h"


SDL_Window* window;
SDL_Renderer* renderer;
const char WINDOW_TITLE[] = "SDL2 测试";


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
    window = SDL_CreateWindow(WINDOW_TITLE, 0, 0, 640, 480, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    if (!window) {
        printf("Fail SDL_CreateWindow: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // 创建 renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    int logical_w, logical_h, physical_w, physical_h;
    SDL_GetWindowSize(window, &logical_w, &logical_h);
    SDL_GL_GetDrawableSize(window, &physical_w, &physical_h);
    float scale_x = (float)physical_w / (float)logical_w;
    float scale_y = (float)physical_h / (float)logical_h;
    printf("scale_x: %f, scale_y: %f\n", scale_x, scale_y);

    // 载入
    loadMenuTheme();
    Elem elemTest = {40, {100, 100, 100, 100}, {}};
    loadElemTexture(renderer, &elemTest, "t-../images/test.png");
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

        // renew
        renewElem(&elemTest);

        // draw
        drawElem(renderer, &elemTest);
        SDL_RenderPresent(renderer);
    }

    // 释放
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}