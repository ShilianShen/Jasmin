#include "intel_arr.h"
#include "intel_set.h"
#include "intel_net.h"
#include "entity.h"
#include "action.h"


// INIT & EXIT =========================================================================================================
bool INTEL_InitIntelArr() {
    REQ_CONDITION(INTEL_InitIntelNet(), return false);
    REQ_CONDITION(INTEL_InitIntelSet(), return false);
    return true;
}
void INTEL_ExitIntelArr() {
    INTEL_ExitIntelNet();
    INTEL_ExitIntelSet();
}


// GET & SET ===========================================================================================================
static Intel* INTEL_GetIntelSet(const int len) {
    REQ_CONDITION(len > 0, return NULL);

    Intel* intelSet = calloc(len, sizeof(Intel));
    REQ_CONDITION(intelSet != NULL, return NULL);

    return intelSet;
}
bool INTEL_AppendIntelArr(IntelArr* intelArr, const Intel intel) {
    REQ_CONDITION(intelArr->arr != NULL, return false);

    for (int i = 0; i < intelArr->len; i++) {
        if (intelArr->arr[i].effective == false) {
            intelArr->arr[i] = intel;
            return true;
        }
    }
    const int len = (int)(intelArr->len * 1.5);
    Intel* intelSet = INTEL_GetIntelSet(len);
    REQ_CONDITION(intelSet != NULL, return false);

    for (int i = 0; i < intelArr->len; i++) {
        intelSet[i] = intelArr->arr[i];
    }
    intelSet[intelArr->len] = intel;

    free(intelArr->arr);
    intelArr->arr = intelSet;
    intelArr->len = len;
    INTEL_ResetIntelNet();
    return true;
}


// CREATE & DELETE =====================================================================================================
static bool INTEL_CreateIntelArr_RK(IntelArr* intelArr) {
    memset(intelArr, 0, sizeof(IntelArr));

    intelArr->len = 10;
    intelArr->arr = INTEL_GetIntelSet(intelArr->len);
    REQ_CONDITION(intelArr->arr != NULL, return false);

    return true;
}
IntelArr* INTEL_CreateIntelArr() {
    IntelArr* intelNet = malloc(sizeof(IntelArr));
    REQ_CONDITION(intelNet != NULL, return NULL);
    REQ_CONDITION(INTEL_CreateIntelArr_RK(intelNet), intelNet = INTEL_DeleteIntelArr(intelNet));
    return intelNet;
}
IntelArr* INTEL_DeleteIntelArr(IntelArr* intelArr) {
    if (intelArr != NULL) {
        if (intelArr->arr != NULL) {
            free(intelArr->arr);
            intelArr->arr = NULL;
        }
        free(intelArr);
    }
    intelArr = NULL;
    return NULL;
}


// RENEW ===============================================================================================================
bool INTEL_RenewIntelArr(IntelArr* intelArr) {
    (netMode ? INTEL_RenewIntelNet : INTEL_RenewIntelSet)(intelArr);
    return true;
}


// DRAW ================================================================================================================
typedef struct {const char* s[6];} SetData;
bool INTEL_DrawIntelArr(IntelArr* intelArr) {
    netMode ? INTEL_DrawIntelNet(intelArr) : INTEL_DrawIntelSet();
    return true;
}