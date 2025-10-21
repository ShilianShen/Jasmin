#ifndef NET_H
#define NET_H


#include "intel.h"


extern Trig trigMove;


void INTEL_ResetIntelNet();


bool INTEL_InitIntelNet();
bool INTEL_RenewIntelNet();
bool INTEL_DrawIntelNet();
void INTEL_ExitIntelNet();


#endif //NET_H