#ifndef INTEL_H
#define INTEL_H


#include "../basic/basic.h"
#include "../debug/debug.h"
#include "../perph/perph.h"


extern bool netMode;
extern const char* INTEL_JUDGE_STRING[];
extern const char* INTEL_STATE_STRING[];
extern const SDL_Color JUDGE_COLOR[];
extern const SDL_Color STATE_COLOR[];


extern TTF_Font *entityFont, *actionFont, *setFont;
#define ACTION_FONT "../res/font/IBMPlexMono-Medium.ttf", 24
#define ENTITY_FONT "../res/font/Courier New.ttf", 48
#define SET_FONT "../res/font/Courier New.ttf", 48


typedef enum {
    STATE_UNKNOWN,
    STATE_TRUE,
    STATE_FALSE,
    STATE_PARADOX,
    NUM_STATES
} IntelState;
typedef enum {
    JUDGE_MANU,
    JUDGE_AUTO,
    NUM_JUDGES,
} IntelJudge;
typedef struct {
    bool effective;
    int subject, action, object;
    IntelJudge judge;
    IntelState state;

    SDL_FRect rect;
    bool visible;
} Intel;


SDL_FPoint INTEL_GetScaledPos(SDL_FPoint pos);
SDL_FPoint INTEL_GetDescalePos(SDL_FPoint pos);
IntelState INTEL_GetAutoState(Intel intel);
const char* INTEL_GetStrIntel(Intel intel);


bool INTEL_Init();
bool INTEL_Renew();
bool INTEL_Draw();
void INTEL_Exit();


#endif //INTEL_H