#define TEMPO_EXTERN_TABLE
#include "env.h"
#include "../include/jasmin/jasmin.h"



static const Env ENV_ARRAY[] = {
    {"BASIC", &BASIC_Init, &BASIC_Renew, NULL, &BASIC_Exit},
    {"DEBUG", &DEBUG_Init, &DEBUG_Renew, &DEBUG_Draw, &DEBUG_Exit},
    {"DEVICE", NULL, &DEVICE_Renew, &DEVICE_Draw, NULL},
    {"TEMPO", &TEMPO_Init, &TEMPO_Renew, &TEMPO_Draw, &TEMPO_Exit},
};
static const int LEN_ENV_ARRAY = sizeof(ENV_ARRAY) / sizeof(Env);


int main() {
    running = Init(LEN_ENV_ARRAY, ENV_ARRAY);
    while (running) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_EVENT_QUIT: running = false; break;
                default: break;
            }
        }
        running = running && Renew(LEN_ENV_ARRAY, ENV_ARRAY);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        running = running && Draw(LEN_ENV_ARRAY, ENV_ARRAY);

        SDL_RenderPresent(renderer);
    }
    Exit(LEN_ENV_ARRAY, ENV_ARRAY);
}
