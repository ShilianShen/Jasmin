// #define MINIAUDIO_IMPLEMENTATION
// #include "../include/jasmin/jasmin.h"
#include "../src/basic/basic.h"
#include "../src/debug/debug.h"
#include "../src/perph/perph.h"
#include "../src/tempo/tempo.h"
#include "../src/lotri/lotri.h"
#include "../src/intel/intel.h"
#include "../src/chess/chess.h"


static const Env ENV_ARRAY[] = {
    {"BASIC", BASIC_Init, BASIC_Renew, BASIC_Draw, BASIC_Exit},
    {"DEBUG", DEBUG_Init, DEBUG_Renew, DEBUG_Draw, DEBUG_Exit},
    {"PERPH", PERPH_Init, PERPH_Renew, PERPH_Draw, PERPH_Exit},
    // {"TEMPO", TEMPO_Init, TEMPO_Renew, TEMPO_Draw, TEMPO_Exit},
    {"INTEL", INTEL_Init, INTEL_Renew, NULL, INTEL_Exit},
    {"LOTRI", LOTRI_Init, LOTRI_Renew, NULL, LOTRI_Exit},
    {"CHESS", VILLA_Init, VILLA_Renew, VILLA_Draw, VILLA_Exit},
};
static const int LEN_ENV_ARRAY = len_of(ENV_ARRAY);


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

        SDL_SetRenderDrawColor(renderer, 0, 64, 0, 255);
        SDL_RenderClear(renderer);

        running = running && BASIC_DrawEnv(LEN_ENV_ARRAY, ENV_ARRAY);

        SDL_RenderPresent(renderer);
    }

    BASIC_ExitEnv(LEN_ENV_ARRAY, ENV_ARRAY);
}
