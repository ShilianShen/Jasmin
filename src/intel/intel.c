#include "intel.h"

#include "../debug/debug.h"


const char* INTEL_STATE_STRING[INTEL_NUM_STATES] = {
    [INTEL_STATE_NULL] = "NULL",
    [INTEL_STATE_SRC_FALSE] = "SRC_FALSE",
    [INTEL_STATE_SRC_TRUE] = "SRC_TRUE",
    [INTEL_STATE_UNKNOWN] = "UNKNOWN",
};
IntelNet* testIntelNet = NULL;


enum Entity {
    ENTITY_0,
    ENTITY_1,
    ENTITY_2,
    ENTITY_3,
    NUM_ENTITIES
};
typedef struct {
    bool visible;
    const char* name;
    SDL_FPoint position;
} EntityInfo;
EntityInfo* entityInfoSet = NULL;


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


// DRAW ================================================================================================================
bool INTEL_DrawIntelNet(IntelNet* intelNet) {
    REQ_CONDITION(intelNet->intelSet != NULL, return false);

    SDL_FRect rects[NUM_ENTITIES];

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderRects(renderer, rects, NUM_ENTITIES);
    SDL_RenderRect(renderer, &windowRect);
    return true;
}


// INIT & EXIT =========================================================================================================
bool INTEL_Init() {
    entityInfoSet = calloc(NUM_ENTITIES, sizeof(EntityInfo));
    REQ_CONDITION(entityInfoSet != NULL, return false);

    testIntelNet = INTEL_CreateIntelNet();
    INTEL_AppendIntelNet(testIntelNet, (Intel){INTEL_STATE_SRC_TRUE, 0, 1, 1});
    INTEL_AppendIntelNet(testIntelNet, (Intel){INTEL_STATE_SRC_TRUE, 0, 1, 2});
    for (int i = 0; i < testIntelNet->len; i++) {
        printf("%s: %d, %d, %d\n", __func__,
            testIntelNet->intelSet[i].subject,
            testIntelNet->intelSet[i].action,
            testIntelNet->intelSet[i].object
            );
    }

    return true;
}
void INTEL_Exit() {
    testIntelNet = INTEL_DeleteIntelNet(testIntelNet);

    free(entityInfoSet); entityInfoSet = NULL;
}


// RENEW ===============================================================================================================
bool INTEL_Renew() {
    return true;
}


// DRAW ================================================================================================================
bool INTEL_Draw() {
    INTEL_DrawIntelNet(testIntelNet);

    return true;
}
