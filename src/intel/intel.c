#include "intel.h"
#include "entity.h"
#include "action.h"


const char* INTEL_STATE_STRING[INTEL_NUM_STATES] = {
    [INTEL_STATE_NULL] = "NULL",
    [INTEL_STATE_MANU_F] = "SRC_FALSE",
    [INTEL_STATE_MANU_T] = "SRC_TRUE",
    [INTEL_STATE_UNKNOWN] = "UNKNOWN",
};
IntelNet* testIntelNet = NULL;
const SDL_FPoint scale = {500, 300};
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


static IntelState INTEL_GetAutoState_OneWay(const Intel intel1) {
    // intel1: subject1 --action-> object1
    for (int k = 0; k < intelNetNow->len; k++) {
        const Intel intel2 = intelNetNow->intelSet[k];

        // intel2: subject1 --action-> object1
        if (intel1.subject == intel2.subject && intel1.object == intel2.object && intel1.action == intel2.action) {
            switch (intel2.state) {
                case INTEL_STATE_MANU_T: case INTEL_STATE_AUTO_T: return INTEL_STATE_AUTO_T;
                case INTEL_STATE_MANU_F: case INTEL_STATE_AUTO_F: return INTEL_STATE_AUTO_F;
                default: break;
            }
        }

        if (intel2.state != INTEL_STATE_MANU_T && intel2.state != INTEL_STATE_AUTO_T) continue;

        int subject3;
        switch (intel2.action) {
            // intel2: subject1/2 --is-> subject2/1
            case ACTION_IS: {
                if (intel2.subject == intel1.subject) subject3 = intel2.object;
                else if (intel2.object == intel1.subject) subject3 = intel2.subject;
                else continue;
                break;
            }

            // intel2: entity1 --belong-> entity2
            case ACTION_BELONG: {
                if (intel2.subject == intel1.subject) {
                    subject3 = intel2.object;
                }
                else continue;
                break;
            }
            default: continue;
        }

        for (int n = 0; n < intelNetNow->len; n++) {
            // intel3: subject3 --action-> object1
            const Intel intel3 = intelNetNow->intelSet[n];

            if (intel3.subject != subject3 || intel3.action != intel1.action || intel3.object != intel1.object) continue;

            switch (intel3.state) {
                case INTEL_STATE_MANU_T: case INTEL_STATE_AUTO_T: return INTEL_STATE_AUTO_T;
                case INTEL_STATE_MANU_F: case INTEL_STATE_AUTO_F: return INTEL_STATE_AUTO_F;
                default: continue;
            }
        }
    }
    return INTEL_STATE_AUTO_UNKNOWN;
}
IntelState INTEL_GetAutoState(const Intel intel1) {
    if (actionSet[intel1.action].type == ACTION_TYPE_TWO_WAY) {
        Intel intel2 = intel1;
        intel2.subject = intel1.object;
        intel2.object = intel1.subject;
        const IntelState state1 = INTEL_GetAutoState_OneWay(intel1);
        const IntelState state2 = INTEL_GetAutoState_OneWay(intel2);
        if (state1 == INTEL_STATE_AUTO_T || state2 == INTEL_STATE_AUTO_T) return INTEL_STATE_AUTO_T;
        if (state1 == INTEL_STATE_AUTO_F || state2 == INTEL_STATE_AUTO_F) return INTEL_STATE_AUTO_F;
        return INTEL_STATE_AUTO_UNKNOWN;
    }
    return INTEL_GetAutoState_OneWay(intel1);
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
    INTEL_AppendIntelNet(testIntelNet, (Intel){INTEL_STATE_MANU_T, ENTITY_SOMEONE, ACTION_IS, ENTITY_SOCRATES});
    INTEL_AppendIntelNet(testIntelNet, (Intel){INTEL_STATE_MANU_T, ENTITY_SOCRATES, ACTION_BELONG, ENTITY_HUMAN});
    INTEL_AppendIntelNet(testIntelNet, (Intel){INTEL_STATE_MANU_T, ENTITY_HUMAN, ACTION_WILL, ENTITY_DEATH});

    // INTEL_AppendIntelNet(testIntelNet, (Intel){INTEL_STATE_AUTO_UNKNOWN, ENTITY_SOMEONE, ACTION_BELONG, ENTITY_HUMAN});
    INTEL_AppendIntelNet(testIntelNet, (Intel){INTEL_STATE_AUTO_UNKNOWN, ENTITY_SOCRATES, ACTION_WILL, ENTITY_DEATH});
    INTEL_AppendIntelNet(testIntelNet, (Intel){INTEL_STATE_AUTO_UNKNOWN, ENTITY_SOMEONE, ACTION_WILL, ENTITY_DEATH});
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
    INTEL_RenewAction();
    return true;
}


// DRAW ================================================================================================================
bool INTEL_Draw() {
    INTEL_DrawAction();
    INTEL_DrawEntity();
    return true;
}
