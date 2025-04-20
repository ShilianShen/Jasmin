#include <SDL2/SDL.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    // 初始化 SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init 失败: %s\n", SDL_GetError());
        return 1;
    }

    // 创建窗口
    SDL_Window* window = SDL_CreateWindow(
        "SDL2 测试",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        640, 480,
        SDL_WINDOW_SHOWN
    );
    if (!window) {
        printf("SDL_CreateWindow 失败: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // 主循环
    SDL_Event event;
    int running = 1;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }
        // 这里可以添加渲染代码
    }

    // 清理资源
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}