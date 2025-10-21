#include "intel.h"
#include "intel_arr.h"
#include "entity.h"
#include "action.h"
#include "net.h"
#include "set.h"


TTF_Font *entityFont = NULL, *setFont = NULL;
bool netMode = true;
const char* INTEL_JUDGE_STRING[NUM_JUDGES] = {
    [JUDGE_AUTO] = "AUTO",
    [JUDGE_MANU] = "MANU",
};
const char* INTEL_STATE_STRING[NUM_STATES] = {
    [STATE_UNKNOWN] = "UNKNOWN",
    [STATE_TRUE] = "TRUE",
    [STATE_FALSE] = "FALSE",
    [STATE_PARADOX] = "PARADOX",
};
static IntelArr* testIntelArr = NULL;
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
    for (int k = 0; k < intelArrNow->len; k++) {
        const Intel intel2 = intelArrNow->arr[k];

        // intel2: subject1 --action-> object1
        if (intel1.subject == intel2.subject && intel1.object == intel2.object && intel1.action == intel2.action) {
            // switch (intel2.state) {
            //     case STATE_MANU_T: case STATE_AUTO_T: return STATE_AUTO_T;
            //     case STATE_MANU_F: case STATE_AUTO_F: return STATE_AUTO_F;
            //     default: break;
            // }
        }

        // if (intel2.state != STATE_MANU_T && intel2.state != STATE_AUTO_T) continue;

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

        for (int n = 0; n < intelArrNow->len; n++) {
            // intel3: subject3 --action-> object1
            const Intel intel3 = intelArrNow->arr[n];

            if (intel3.subject != subject3 || intel3.action != intel1.action || intel3.object != intel1.object) continue;

            // switch (intel3.state) {
            //     case STATE_MANU_T: case STATE_AUTO_T: return STATE_AUTO_T;
            //     case STATE_MANU_F: case STATE_AUTO_F: return STATE_AUTO_F;
            //     default: continue;
            // }
        }
    }
    return STATE_UNKNOWN;
}
IntelState INTEL_GetAutoState(const Intel intel1) {
    if (actionSet[intel1.action].type == ACTION_TYPE_TWO_WAY) {
        Intel intel2 = intel1;
        intel2.subject = intel1.object;
        intel2.object = intel1.subject;
        const IntelState state1 = INTEL_GetAutoState_OneWay(intel1);
        const IntelState state2 = INTEL_GetAutoState_OneWay(intel2);
        // if (state1 == STATE_AUTO_T || state2 == STATE_AUTO_T) return STATE_AUTO_T;
        // if (state1 == STATE_AUTO_F || state2 == STATE_AUTO_F) return STATE_AUTO_F;
        // return STATE_AUTO_U;
    }
    return INTEL_GetAutoState_OneWay(intel1);
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


// TRIG ================================================================================================================
static void INTEL_ChangeMode(const void* para) {netMode = !netMode;}
const Trig trigChangeMode = {INTEL_ChangeMode, NULL, false};


// INIT & EXIT =========================================================================================================
bool INTEL_Init() {
    entityFont = TTF_OpenFont(ENTITY_FONT); REQ_CONDITION(entityFont != NULL, return false);
    setFont = TTF_OpenFont(SET_FONT); REQ_CONDITION(setFont != NULL, return false);

    INTEL_InitIntelNet();
    INTEL_InitIntelSet();

    testIntelArr = INTEL_CreateIntelArr();
    INTEL_AppendIntelArr(testIntelArr, (Intel){
        true,
        ENTITY_SOCRATES, ACTION_BELONG, ENTITY_HUMAN,
        JUDGE_MANU, STATE_TRUE
    });
    INTEL_AppendIntelArr(testIntelArr, (Intel){
        true,
        ENTITY_SOCRATES, ACTION_CAN, ENTITY_FLY,
        JUDGE_MANU, STATE_FALSE
    });
    INTEL_AppendIntelArr(testIntelArr, (Intel){
        true,
        ENTITY_HUMAN, ACTION_WILL, ENTITY_DEATH,
        JUDGE_MANU, STATE_TRUE
    });
    INTEL_AppendIntelArr(testIntelArr, (Intel){
        true,
        ENTITY_SOCRATES, ACTION_WILL, ENTITY_DEATH,
        JUDGE_AUTO, STATE_UNKNOWN
    });
    INTEL_AppendIntelArr(testIntelArr, (Intel){
        true,
        ENTITY_HUMAN, ACTION_CAN, ENTITY_FLY,
        JUDGE_AUTO, STATE_UNKNOWN
    });

    intelArrNow = testIntelArr;
    REQ_CONDITION(INTEL_InitEntity(), return false);
    REQ_CONDITION(INTEL_InitAction(), return false);
    return true;
}
void INTEL_Exit() {
    INTEL_ExitIntelNet();
    INTEL_ExitIntelSet();
    INTEL_ExitEntity();
    INTEL_ExitAction();
    intelArrNow = NULL;
    testIntelArr = INTEL_DeleteIntelArr(testIntelArr);

    TTF_CloseFont(entityFont); entityFont = NULL;
    TTF_CloseFont(setFont); setFont = NULL;
}


// RENEW ===============================================================================================================
bool INTEL_Renew() {
    INTEL_RenewIntelArr();
    PERPH_SetKeyTrig(SDL_SCANCODE_TAB, &trigChangeMode);
    return true;
}


// DRAW ================================================================================================================
bool INTEL_Draw() {
    DEBUG_SendMessageL("%s: mode: %s\n", __func__, netMode ? "NET" : "SET");
    INTEL_DrawIntelArr();
    return true;
}
