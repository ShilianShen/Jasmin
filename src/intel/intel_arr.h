#ifndef INTEL_NET_H
#define INTEL_NET_H


#include "intel.h"


typedef struct {int len; Intel* arr;} IntelArr;


// extern IntelArr* intelArrNow;


IntelArr* INTEL_CreateIntelArr();
IntelArr* INTEL_DeleteIntelArr(IntelArr* intelArr);


bool INTEL_InitIntelArr();
bool INTEL_RenewIntelArr(IntelArr* intelArr);
bool INTEL_DrawIntelArr(IntelArr* intelArr);
void INTEL_ExitIntelArr();


bool INTEL_AppendIntelArr(IntelArr* intelArr, Intel intel);


#endif //INTEL_NET_H