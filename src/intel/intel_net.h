#ifndef NET_H
#define NET_H


#include "intel_arr.h"


extern Trig trigMove;


void INTEL_ResetIntelNet();


bool INTEL_InitIntelNet();
bool INTEL_RenewIntelNet(IntelArr* intelArr);
bool INTEL_DrawIntelNet(IntelArr* intelArr);
void INTEL_ExitIntelNet();


#endif //NET_H