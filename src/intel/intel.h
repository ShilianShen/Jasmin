#ifndef INTEL_H
#define INTEL_H


#include "../basic/basic.h"
#include "../debug/debug.h"
#include "../perph/perph.h"


extern bool netMode;
extern const char* JUDGE_NAMES[];
extern const char* STATE_NAMES[];
extern const SDL_Color JUDGE_COLORS[];
extern const SDL_Color STATE_COLORS[];


typedef enum {STATE_UNKNOWN, STATE_TRUE, STATE_FALSE, STATE_PARADOX, NUM_STATES} IntelState;
typedef enum {JUDGE_MANU, JUDGE_AUTO, NUM_JUDGES} IntelJudge;
typedef struct {
    bool effective;
    int subject, action, object;
    IntelJudge judge;
    IntelState state;

    bool visible;
    SDL_FRect rect;
} Intel;


const char* INTEL_GetStrIntel(Intel intel);


bool INTEL_Init();
bool INTEL_Renew();
bool INTEL_Draw();
void INTEL_Exit();


#endif //INTEL_H