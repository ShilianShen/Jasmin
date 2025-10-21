#include "intel_arr.h"
#include "intel_set.h"
#include "intel_net.h"
#include "entity.h"
#include "action.h"


IntelArr* intelArrNow = NULL;


// INIT & EXIT =========================================================================================================
bool INTEL_InitIntelArr() {
    INTEL_InitIntelNet();
    INTEL_InitIntelSet();
    return true;
}
void INTEL_ExitIntelArr() {
    INTEL_ExitIntelNet();
    INTEL_ExitIntelSet();
}


// RENEW ===============================================================================================================
bool INTEL_RenewIntelArr() {
    if (netMode) {
        INTEL_RenewIntelNet();
    }
    return true;
}


// DRAW ================================================================================================================
typedef struct {const char* s[6];} SetData;
bool INTEL_DrawIntelArr() {
    return netMode ? INTEL_DrawIntelNet() : INTEL_DrawIntelSet();
}