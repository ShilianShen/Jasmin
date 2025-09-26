#ifndef I_ENG_H
#define I_ENG_H


#include "../basic/basic.h"


typedef struct Intel Intel;
typedef struct IntelNet IntelNet;
struct Intel {bool yes; int subject; int action; int object;};
struct IntelNet {int len; Intel* intelSet;};


IntelNet* INTEL_CreateIntelNet();
IntelNet* INTEL_DeleteIntelNet(IntelNet* intelNet);


bool INTEL_Init();
bool INTEL_Renew();
bool INTEL_Draw();
void INTEL_Exit();


#endif //I_ENG_H