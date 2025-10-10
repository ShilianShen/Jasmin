#ifndef ACTION_H
#define ACTION_H


#include "intel.h"


enum ActionId {
    ACTION_UNKNOWN,
    ACTION_IS,
    ACTION_BELONG,
    ACTION_WILL,
    NUM_ACTIONS,
};
typedef struct {
    const char* name;
    SDL_Texture* tex;
} Action;


extern Action actionSet[NUM_ACTIONS];


bool INTEL_InitAction();
void INTEL_ExitAction();
bool INTEL_DrawAction(const IntelNet* intelNet);


#endif //ACTION_H