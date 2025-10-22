#ifndef SET_H
#define SET_H


#include "intel_arr.h"


SDL_FRect INTEL_GetIntelSetBckRect();


void INTEL_TrigChangeVisible(void* para);
void INTEL_TrigChangeJudge(void* para);
void INTEL_TrigChangeState(void* para);
void INTEL_TrigSortVisible(void* para);
void INTEL_TrigSortJudge(void* para);
void INTEL_TrigSortSubject(void* para);
void INTEL_TrigSortAction(void* para);
void INTEL_TrigSortObject(void* para);
void INTEL_TrigSortState(void* para);


bool INTEL_InitIntelSet();
bool INTEL_RenewIntelSet(IntelArr* intelArr);
bool INTEL_DrawIntelSet();
void INTEL_ExitIntelSet();


#endif //SET_H