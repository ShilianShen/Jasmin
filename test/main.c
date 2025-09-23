// #define MINIAUDIO_IMPLEMENTATION
#include "../include/jasmin/jasmin.h"



static const Env ENV_ARRAY[] = {
    {"BASIC", BASIC_Init, BASIC_Renew, BASIC_Draw, BASIC_Exit},
    {"DEBUG", DEBUG_Init, DEBUG_Renew, DEBUG_Draw, DEBUG_Exit},
    {"PERPH", PERPH_Init, PERPH_Renew, PERPH_Draw, PERPH_Exit},
    {"TEMPO", TEMPO_Init, TEMPO_Renew, TEMPO_Draw, TEMPO_Exit},
    {"LOTRI", LOTRI_Init, LOTRI_Renew, LOTRI_Draw, LOTRI_Exit},
    {"VILLA", VILLA_Init, VILLA_Renew, VILLA_Draw, VILLA_Exit},
};
static const int LEN_ENV_ARRAY = sizeof(ENV_ARRAY) / sizeof(Env);



int main() {
    running = BASIC_InitEnv(LEN_ENV_ARRAY, ENV_ARRAY);

    while (running) {
        while (SDL_PollEvent(&sdl_event)) {
            switch (sdl_event.type) {
                case SDL_EVENT_QUIT: running = false; break;
                default: break;
            }
        }
        running = running && BASIC_RenewEnv(LEN_ENV_ARRAY, ENV_ARRAY);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        running = running && BASIC_DrawEnv(LEN_ENV_ARRAY, ENV_ARRAY);

        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }

    BASIC_ExitEnv(LEN_ENV_ARRAY, ENV_ARRAY);
}
