#ifndef SET_H
#define SET_H


#include "intel_arr.h"


SDL_FRect INTEL_GetIntelSetBckRect();


void INTEL_TrigChangeVisible(TrigPara para);
void INTEL_TrigChangeJudge(TrigPara para);
void INTEL_TrigChangeState(TrigPara para);
void INTEL_TrigSortVisible(TrigPara para);
void INTEL_TrigSortJudge(TrigPara para);
void INTEL_TrigSortSubject(TrigPara para);
void INTEL_TrigSortAction(TrigPara para);
void INTEL_TrigSortObject(TrigPara para);
void INTEL_TrigSortState(TrigPara para);


bool INTEL_InitIntelSet();
bool INTEL_RenewIntelSet(IntelArr* intelArr);
bool INTEL_DrawIntelSet();
void INTEL_ExitIntelSet();


#endif //SET_H