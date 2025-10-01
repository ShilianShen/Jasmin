#include "intel.h"


static bool INTEL_CreateIntelNet_RK(IntelNet* intelNet, const int len) {
    memset(intelNet, 0, sizeof(IntelNet));

    intelNet->len = len;
    intelNet->intelSet = calloc(intelNet->len, sizeof(Intel));

    REQ_CONDITION(intelNet->intelSet != NULL, return false);
    return true;
}
IntelNet* INTEL_CreateIntelNet() {
    IntelNet* intelNet = malloc(sizeof(IntelNet));
    REQ_CONDITION(intelNet != NULL, return NULL);
    REQ_CONDITION(INTEL_CreateIntelNet_RK(intelNet, 10), intelNet = INTEL_DeleteIntelNet(intelNet));
    return intelNet;
}
IntelNet* INTEL_DeleteIntelNet(IntelNet* intelNet) {
    if (intelNet != NULL) {
        if (intelNet->intelSet != NULL) {
            free(intelNet->intelSet);
            intelNet->intelSet = NULL;
        }
        free(intelNet);
    }
    intelNet = NULL;
    return NULL;
}
bool INTEL_AppendIntelNet(IntelNet* intelNet, const Intel intel) {
    REQ_CONDITION(intelNet->intelSet != NULL, return false);
    REQ_CONDITION(intel.yes, return false);

    for (int i = 0; i < intelNet->len; i++) {
        if (intelNet->intelSet[i].yes == false) {
            intelNet->intelSet[i] = intel;
            return true;
        }
    }

    const int len = (int)(intelNet->len * 1.5);


    return true;
}


bool INTEL_Init() {
    return true;
}
bool INTEL_Renew() {
    return true;
}
bool INTEL_Draw() {
    return true;
}
void INTEL_Exit() {
}