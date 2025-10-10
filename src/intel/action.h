#ifndef ACTION_H
#define ACTION_H


#include "intel.h"


enum ActionId {
    ACTION_UNKNOWN,
    ACTION_EQUAL,
    ACTION_BELONG,
    ACTION_WILL,
    ACTION_0,
    ACTION_1,
    ACTION_2,
    ACTION_3,
    NUM_ACTIONS,
};
typedef struct {
    const char* name;
    SDL_Texture* tex;
} Action;


extern Action actionSet[NUM_ACTIONS];


bool INTEL_InitAction();
void INTEL_ExitAction();


#endif //ACTION_H