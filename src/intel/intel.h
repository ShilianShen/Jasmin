#ifndef INTEL_H
#define INTEL_H


#include "../basic/basic.h"
#include "../debug/debug.h"


typedef enum {
    INTEL_STATE_NULL,
    INTEL_STATE_SRC_FALSE,
    INTEL_STATE_SRC_TRUE,
    INTEL_STATE_UNKNOWN,
    INTEL_STATE_AUTO_UNKNOWN,
    INTEL_STATE_AUTO_TRUE,
    INTEL_STATE_AUTO_FALSE,
    INTEL_NUM_STATES
} IntelState;
typedef struct {
    IntelState state;
    int subject, action, object;
} Intel;
typedef struct {
    int len;
    Intel* intelSet;
} IntelNet;


extern IntelNet* intelNetNow;


IntelNet* INTEL_CreateIntelNet();
IntelNet* INTEL_DeleteIntelNet(IntelNet* intelNet);
SDL_FPoint INTEL_GetScaledPos(SDL_FPoint pos);
IntelState INTEL_GetAutoState(Intel intel1);


bool INTEL_Init();
bool INTEL_Renew();
bool INTEL_Draw();
void INTEL_Exit();


#endif //INTEL_H