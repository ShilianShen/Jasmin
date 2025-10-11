#ifndef ACTION_H
#define ACTION_H


#include "intel.h"


typedef enum {
    ACTION_UNKNOWN,
    ACTION_IS,
    ACTION_BELONG,
    ACTION_WILL,
    NUM_ACTIONS,
} ActionId;
typedef enum {
    ACTION_TYPE_ONE_WAY,
    ACTION_TYPE_TWO_WAY,
    NUM_ACTION_TYPES,
} ActionType;
typedef struct {
    const char* name;
    SDL_Texture* tex;
    ActionType type;
} Action;


extern Action actionSet[NUM_ACTIONS];


bool INTEL_InitAction();
bool INTEL_RenewAction();
bool INTEL_DrawAction();
void INTEL_ExitAction();


#endif //ACTION_H