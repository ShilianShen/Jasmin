#ifndef INTEL_H
#define INTEL_H


#include "../basic/basic.h"
#include "../debug/debug.h"
#include "../perph/perph.h"


typedef enum {
    INTEL_STATE_NULL,

    INTEL_STATE_MANU_UNKNOWN,
    INTEL_STATE_MANU_T,
    INTEL_STATE_MANU_F,

    INTEL_STATE_AUTO_UNKNOWN,
    INTEL_STATE_AUTO_T,
    INTEL_STATE_AUTO_F,
    INTEL_STATE_AUTO_PARADOX,

    INTEL_NUM_STATES
} IntelState;
typedef struct {
    IntelState state;
    int subject, action, object;
    SDL_FRect rect;
    bool visible;
} Intel;


SDL_FPoint INTEL_GetScaledPos(SDL_FPoint pos);
SDL_FPoint INTEL_GetDescalePos(SDL_FPoint pos);
IntelState INTEL_GetAutoState(Intel intel1);
const char* INTEL_GetStrIntel(Intel intel);


bool INTEL_Init();
bool INTEL_Renew();
bool INTEL_Draw();
void INTEL_Exit();


#endif //INTEL_H