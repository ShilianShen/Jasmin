#ifndef INTEL_NET_H
#define INTEL_NET_H


#include "intel.h"


typedef struct {
    int len;
    Intel* intelSet;
} IntelNet;


extern IntelNet* intelNetNow;


IntelNet* INTEL_CreateIntelNet();
IntelNet* INTEL_DeleteIntelNet(IntelNet* intelNet);
bool INTEL_RenewIntelNet();
bool INTEL_DrawIntelNet();


bool INTEL_AppendIntelNet(IntelNet* intelNet, Intel intel);


#endif //INTEL_NET_H