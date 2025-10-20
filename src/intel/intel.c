#include "intel.h"
#include "intel_net.h"
#include "entity.h"
#include "action.h"


const char* INTEL_JUDGE_STRING[NUM_JUDGES] = {
    [JUDGE_AUTO] = "AUTO",
    [JUDGE_MANU] = "MANU",
};
const char* INTEL_STATE_STRING[NUM_STATES] = {
    [STATE_NULL] = "NULL",
    [STATE_MANU_F] = "SRC_FALSE",
    [STATE_MANU_T] = "SRC_TRUE",
    [STATE_MANU_U] = "UNKNOWN",
};
static IntelNet* testIntelNet = NULL;
const SDL_FPoint scale = {500, 300};


// GET & SET ===========================================================================================================
const char* INTEL_GetStrIntel(const Intel intel) {
    static char string[64];
    static size_t len = 0;
    if (len == 0) len = len_of(string);
    snprintf(string, len, "\"%s %s %s.\" is %s by %s",
        entitySet[intel.subject].name,
        actionSet[intel.action].name,
        entitySet[intel.object].name,
        INTEL_STATE_STRING[intel.state],
        INTEL_JUDGE_STRING[intel.judge]
        );
    return string;
}
static Intel* INTEL_GetIntelSet(const int len) {
    REQ_CONDITION(len > 0, return NULL);

    Intel* intelSet = calloc(len, sizeof(Intel));
    REQ_CONDITION(intelSet != NULL, return NULL);

    return intelSet;
}
bool INTEL_AppendIntelNet(IntelNet* intelNet, const Intel intel) {
    REQ_CONDITION(intelNet->intelSet != NULL, return false);

    for (int i = 0; i < intelNet->len; i++) {
        if (intelNet->intelSet[i].state == STATE_NULL) {
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
    INTEL_ResetEntity();
    return true;
}
SDL_FPoint INTEL_GetScaledPos(const SDL_FPoint pos) {
    const SDL_FPoint scaledPos = {
        windowRect.x + windowRect.w / 2 + scale.x * pos.x,
        windowRect.y + windowRect.h / 2 + scale.y * pos.y
    };
    return scaledPos;
}
SDL_FPoint INTEL_GetDescalePos(const SDL_FPoint pos) {
    const SDL_FPoint descalePos = {
        (pos.x - windowRect.x - windowRect.w / 2) / scale.x,
        (pos.y - windowRect.y - windowRect.h / 2) / scale.y,
    };
    return descalePos;
}


static IntelState INTEL_GetAutoState_OneWay(const Intel intel1) {
    // intel1: subject1 --action-> object1
    for (int k = 0; k < intelNetNow->len; k++) {
        const Intel intel2 = intelNetNow->intelSet[k];

        // intel2: subject1 --action-> object1
        if (intel1.subject == intel2.subject && intel1.object == intel2.object && intel1.action == intel2.action) {
            switch (intel2.state) {
                case STATE_MANU_T: case STATE_AUTO_T: return STATE_AUTO_T;
                case STATE_MANU_F: case STATE_AUTO_F: return STATE_AUTO_F;
                default: break;
            }
        }

        if (intel2.state != STATE_MANU_T && intel2.state != STATE_AUTO_T) continue;

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
                case STATE_MANU_T: case STATE_AUTO_T: return STATE_AUTO_T;
                case STATE_MANU_F: case STATE_AUTO_F: return STATE_AUTO_F;
                default: continue;
            }
        }
    }
    return STATE_AUTO_U;
}
IntelState INTEL_GetAutoState(const Intel intel1) {
    if (actionSet[intel1.action].type == ACTION_TYPE_TWO_WAY) {
        Intel intel2 = intel1;
        intel2.subject = intel1.object;
        intel2.object = intel1.subject;
        const IntelState state1 = INTEL_GetAutoState_OneWay(intel1);
        const IntelState state2 = INTEL_GetAutoState_OneWay(intel2);
        if (state1 == STATE_AUTO_T || state2 == STATE_AUTO_T) return STATE_AUTO_T;
        if (state1 == STATE_AUTO_F || state2 == STATE_AUTO_F) return STATE_AUTO_F;
        return STATE_AUTO_U;
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
    INTEL_AppendIntelNet(testIntelNet, (Intel){
        ENTITY_SOCRATES, ACTION_BELONG, ENTITY_HUMAN,
        JUDGE_MANU, STATE_MANU_T
    });
    INTEL_AppendIntelNet(testIntelNet, (Intel){
        ENTITY_SOCRATES, ACTION_CAN, ENTITY_FLY,
        JUDGE_MANU, STATE_MANU_F
    });
    INTEL_AppendIntelNet(testIntelNet, (Intel){
        ENTITY_HUMAN, ACTION_WILL, ENTITY_DEATH,
        JUDGE_MANU, STATE_MANU_T
    });
    INTEL_AppendIntelNet(testIntelNet, (Intel){
        ENTITY_SOCRATES, ACTION_WILL, ENTITY_DEATH,
        JUDGE_AUTO, STATE_AUTO_U
    });
    INTEL_AppendIntelNet(testIntelNet, (Intel){
        ENTITY_HUMAN, ACTION_CAN, ENTITY_FLY,
        JUDGE_AUTO, STATE_AUTO_U
    });

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
    INTEL_RenewIntelNet();
    return true;
}


// DRAW ================================================================================================================
bool INTEL_Draw() {
    INTEL_DrawIntelNet();
    INTEL_DrawEntity();
    return true;
}
