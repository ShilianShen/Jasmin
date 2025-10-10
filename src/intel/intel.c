#include "intel.h"
#include "entity.h"
#include "action.h"


const char* INTEL_STATE_STRING[INTEL_NUM_STATES] = {
    [INTEL_STATE_NULL] = "NULL",
    [INTEL_STATE_SRC_FALSE] = "SRC_FALSE",
    [INTEL_STATE_SRC_TRUE] = "SRC_TRUE",
    [INTEL_STATE_UNKNOWN] = "UNKNOWN",
};
IntelNet* testIntelNet = NULL;
const SDL_FPoint scale = {300, 300};
IntelNet* intelNetNow = NULL;


// GET & SET ===========================================================================================================
static Intel* INTEL_GetIntelSet(const int len) {
    REQ_CONDITION(len > 0, return NULL);

    Intel* intelSet = calloc(len, sizeof(Intel));
    REQ_CONDITION(intelSet != NULL, return NULL);

    return intelSet;
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
SDL_FPoint INTEL_GetScaledPos(const SDL_FPoint pos) {
    const SDL_FPoint scaledPos = {
        windowRect.x + windowRect.w / 2 + scale.x * pos.x,
        windowRect.y + windowRect.h / 2 + scale.y * pos.y
    };
    return scaledPos;
}


// CREATE & DELETE =====================================================================================================
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


// INIT & EXIT =========================================================================================================
bool INTEL_Init() {
    testIntelNet = INTEL_CreateIntelNet();
    INTEL_AppendIntelNet(testIntelNet, (Intel){INTEL_STATE_SRC_TRUE, ENTITY_UNKNOWN, ACTION_IS, ENTITY_SOCRATES});
    INTEL_AppendIntelNet(testIntelNet, (Intel){INTEL_STATE_SRC_TRUE, ENTITY_SOCRATES, ACTION_BELONG, ENTITY_HUMAN});
    INTEL_AppendIntelNet(testIntelNet, (Intel){INTEL_STATE_SRC_FALSE, ENTITY_HUMAN, ACTION_WILL, ENTITY_DEATH});
    intelNetNow = testIntelNet;
    REQ_CONDITION(INTEL_InitEntity(), return false);
    REQ_CONDITION(INTEL_InitAction(), return false);
    return true;
}
void INTEL_Exit() {
    INTEL_ExitEntity();
    INTEL_ExitAction();
    intelNetNow = NULL;
    testIntelNet = INTEL_DeleteIntelNet(testIntelNet);
}


// RENEW ===============================================================================================================
bool INTEL_Renew() {
    INTEL_RenewEntity();
    return true;
}


// DRAW ================================================================================================================
bool INTEL_Draw() {
    INTEL_DrawAction();
    INTEL_DrawEntity();
    return true;
}
