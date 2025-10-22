#ifndef SET_H
#define SET_H


#include "intel_arr.h"


SDL_FRect INTEL_GetIntelSetBckRect();


bool INTEL_InitIntelSet();
bool INTEL_RenewIntelSet(IntelArr* intelArr);
bool INTEL_DrawIntelSet();
void INTEL_ExitIntelSet();


#endif //SET_H