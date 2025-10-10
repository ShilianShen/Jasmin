#ifndef INTEL_H
#define INTEL_H


#include "../basic/basic.h"
#include "../debug/debug.h"


typedef enum IntelState IntelState;
enum IntelState {
    INTEL_STATE_NULL,
    INTEL_STATE_SRC_FALSE,
    INTEL_STATE_SRC_TRUE,
    INTEL_STATE_UNKNOWN,
    INTEL_NUM_STATES
};
typedef struct Intel Intel;
typedef struct IntelNet IntelNet;
struct Intel {IntelState state; int subject; int action; int object;};
struct IntelNet {
    int len;
    Intel* intelSet;
};
extern const SDL_FPoint scale;


IntelNet* INTEL_CreateIntelNet();
IntelNet* INTEL_DeleteIntelNet(IntelNet* intelNet);


bool INTEL_Init();
bool INTEL_Renew();
bool INTEL_Draw();
void INTEL_Exit();


#endif //INTEL_H