#include "intel.h"



const char* INTEL_STATE_STRING[INTEL_NUM_STATES] = {
    [INTEL_STATE_NULL] = "NULL",
    [INTEL_STATE_SRC_FALSE] = "SRC_FALSE",
    [INTEL_STATE_SRC_TRUE] = "SRC_TRUE",
    [INTEL_STATE_UNKNOWN] = "UNKNOWN",
};
IntelNet* testIntelNet = NULL;

static Intel* INTEL_GetIntelSet(const int len) {
    REQ_CONDITION(len > 0, return NULL);

    Intel* intelSet = calloc(len, sizeof(Intel));
    REQ_CONDITION(intelSet != NULL, return NULL);

    return intelSet;
}
static bool INTEL_CreateIntelNet_RK(IntelNet* intelNet) {
    memset(intelNet, 0, sizeof(IntelNet));

    intelNet->len = 10;
    intelNet->intelSet = INTEL_GetIntelSet(intelNet->len);
    REQ_CONDITION(intelNet->intelSet != NULL, return false);

    return true;
}
IntelNet* INTEL_CreateIntelNet() {
    IntelNet* intelNet = malloc(sizeof(IntelNet));
    REQ_CONDITION(intelNet != NULL, return NULL);
    REQ_CONDITION(INTEL_CreateIntelNet_RK(intelNet), intelNet = INTEL_DeleteIntelNet(intelNet));
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

    for (int i = 0; i < intelNet->len; i++) {
        if (intelNet->intelSet[i].state == INTEL_STATE_NULL) {
            intelNet->intelSet[i] = intel;
            return true;
        }
    }
    const int len = (int)(intelNet->len * 1.5);
    Intel* intelSet = INTEL_GetIntelSet(len);
    REQ_CONDITION(intelSet != NULL, return false);

    for (int i = 0; i < intelNet->len; i++) {
        intelSet[i] = intelNet->intelSet[i];
    }
    intelSet[intelNet->len] = intel;

    free(intelNet->intelSet);
    intelNet->intelSet = intelSet;
    intelNet->len = len;


    return true;
}
bool INTEL_DrawIntelNet(IntelNet* intelNet) {
    REQ_CONDITION(intelNet->intelSet != NULL, return false);
    return true;
}


const char* tabData[] = {
    "0", "1", "2"
};


bool INTEL_Init() {
    testIntelNet = INTEL_CreateIntelNet();
    INTEL_AppendIntelNet(testIntelNet, (Intel){INTEL_STATE_SRC_TRUE, 0, 1, 2});
    return true;
}
bool INTEL_Renew() {
    return true;
}
bool INTEL_Draw() {
    INTEL_DrawIntelNet(testIntelNet);


    return true;
}
void INTEL_Exit() {
    testIntelNet = INTEL_DeleteIntelNet(testIntelNet);
}