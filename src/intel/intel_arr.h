#ifndef INTEL_NET_H
#define INTEL_NET_H


#include "intel.h"


typedef struct {
    int len;
    Intel* arr;
} IntelArr;


extern IntelArr* intelArrNow;


IntelArr* INTEL_CreateIntelNet();
IntelArr* INTEL_DeleteIntelNet(IntelArr* intelNet);
bool INTEL_RenewIntelArr();
bool INTEL_DrawIntelArr();


bool INTEL_AppendIntelNet(IntelArr* intelNet, Intel intel);


#endif //INTEL_NET_H