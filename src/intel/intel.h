#ifndef INTEL_H
#define INTEL_H


#include "../basic/basic.h"
#include "../debug/debug.h"
#include "../perph/perph.h"


extern bool netMode;
extern const char* INTEL_JUDGE_STRING[];
extern const char* INTEL_STATE_STRING[];


typedef enum {
    STATE_NULL,

    STATE_MANU_U,
    STATE_MANU_T,
    STATE_MANU_F,

    STATE_AUTO_U,
    STATE_AUTO_T,
    STATE_AUTO_F,
    STATE_AUTO_P,

    NUM_STATES
} IntelState;
typedef enum {
    JUDGE_MANU,
    JUDGE_AUTO,
    NUM_JUDGES,
} IntelJudge;


typedef struct {
    int subject, action, object;
    IntelJudge judge;
    IntelState state;
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