#include "../include/jasmin/jasmin.h"


typedef struct Env {
    const char* name;
    bool (*init)();
    bool (*renew)();
    bool (*draw)();
    void (*exit)();
} Env;


static const Env ENV_ARRAY[] = {
    {"BASIC", &BASIC_Init, &BASIC_Renew, NULL, &BASIC_Exit},
    {"DEBUG", &DEBUG_Init, &DEBUG_Renew, &DEBUG_Draw, &DEBUG_Exit},
    {"DEVICE", NULL, &DEVICE_Renew, &DEVICE_Draw, NULL},
    {"TEMPO", &TEMPO_Init, &TEMPO_Renew, &TEMPO_Draw, &TEMPO_Exit},
};
static const int LEN_ENV_ARRAY = sizeof(ENV_ARRAY) / sizeof(Env);


static bool Init() {
    for (int i = 0; i < LEN_ENV_ARRAY; i++) {
        if (ENV_ARRAY[i].init != NULL && ENV_ARRAY[i].init() == false) {
            printf("%s: fail in %s.\n", __func__, ENV_ARRAY[i].name);
            return false;
        }
    }
    return true;
}
static bool Renew() {
    for (int i = 0; i < LEN_ENV_ARRAY; i++) {
        if (ENV_ARRAY[i].renew != NULL && ENV_ARRAY[i].renew() == false) {
            printf("%s: fail in %s.\n", __func__, ENV_ARRAY[i].name);
            return false;
        }
    }
    return true;
}
static bool Draw() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    for (int i = LEN_ENV_ARRAY - 1; i >= 0; i--) {
        if (ENV_ARRAY[i].draw != NULL && ENV_ARRAY[i].draw() == false) {
            printf("%s: fail in %s.\n", __func__, ENV_ARRAY[i].name);
            return false;
        }
    }
    SDL_RenderPresent(renderer);
    return true;
}
static void Exit() {
    for (int i = LEN_ENV_ARRAY - 1; i >= 0; i--) {
        if (ENV_ARRAY[i].exit != NULL) {
            ENV_ARRAY[i].exit();
        }
    }
}


int main() {
    running = Init();
    while (running) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_EVENT_QUIT: running = false; break;
                default: break;
            }
        }
        running = running && Renew();
        running = running && Draw();
    }
    Exit();
    return 0;
}
