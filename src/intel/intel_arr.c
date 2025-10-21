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


// RENEW ===============================================================================================================
bool INTEL_RenewIntelArr(IntelArr* intelArr) {
    if (netMode) {
        INTEL_RenewIntelNet(intelArr);
    }
    return true;
}


// DRAW ================================================================================================================
typedef struct {const char* s[6];} SetData;
bool INTEL_DrawIntelArr(IntelArr* intelArr) {
    return netMode ? INTEL_DrawIntelNet(intelArr) : INTEL_DrawIntelSet(intelArr);
}