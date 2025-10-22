#include "intel.h"
#include "intel_arr.h"
#include "entity.h"
#include "action.h"
#include "intel_net.h"
#include "intel_set.h"


bool netMode = true;
const SDL_Color JUDGE_COLORS[NUM_JUDGES] = {
    [JUDGE_AUTO] = {255, 215,   0, 192},
    [JUDGE_MANU] = {255, 255, 255, 255},
};
const SDL_Color STATE_COLORS[NUM_STATES] = {
    [STATE_UNKNOWN] = { 64,  64,  64, 192},
    [STATE_TRUE] = { 32, 128,  32, 192},
    [STATE_FALSE] = {128,  32,  32, 192},
    [STATE_PARADOX] = {  0,   0, 255,   0},
};
const char* JUDGE_NAMES[NUM_JUDGES] = {
    [JUDGE_AUTO] = "AUTO",
    [JUDGE_MANU] = "MANU",
};
const char* STATE_NAMES[NUM_STATES] = {
    [STATE_UNKNOWN] = "UNKNOWN",
    [STATE_TRUE] = "TRUE",
    [STATE_FALSE] = "FALSE",
    [STATE_PARADOX] = "PARADOX",
};
static IntelArr* testIntelArr = NULL;


// GET & SET ===========================================================================================================
const char* INTEL_GetStrIntel(const Intel intel) {
    static char string[64];
    static size_t len = 0;
    if (len == 0) len = len_of(string);
    snprintf(string, len, "\"%s %s %s.\" is %s by %s",
        ENTITY_NAMES[intel.subject],
        ACTION_NAMES[intel.action],
        ENTITY_NAMES[intel.object],
        STATE_NAMES[intel.state],
        JUDGE_NAMES[intel.judge]
        );
    return string;
}
static IntelState INTEL_GetAutoState_OneWay(const Intel intel1) {

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


// TRIG ================================================================================================================
static void INTEL_ChangeMode(void* para) {
    netMode = !netMode;
    INTEL_ResetIntelNet();
}


// INIT & EXIT =========================================================================================================
bool INTEL_Init() {
    INTEL_InitIntelArr();

    testIntelArr = INTEL_CreateIntelArr();
    INTEL_AppendIntelArr(testIntelArr, (Intel){
        true,
        ENTITY_SOCRATES, ACTION_BELONG, ENTITY_HUMAN,
        JUDGE_MANU, STATE_TRUE, true
    });
    INTEL_AppendIntelArr(testIntelArr, (Intel){
        true,
        ENTITY_SOCRATES, ACTION_CAN, ENTITY_FLY,
        JUDGE_MANU, STATE_FALSE, true
    });
    INTEL_AppendIntelArr(testIntelArr, (Intel){
        true,
        ENTITY_HUMAN, ACTION_WILL, ENTITY_DEATH,
        JUDGE_MANU, STATE_TRUE, true
    });
    INTEL_AppendIntelArr(testIntelArr, (Intel){
        true,
        ENTITY_SOCRATES, ACTION_WILL, ENTITY_DEATH,
        JUDGE_AUTO, STATE_UNKNOWN, true
    });
    INTEL_AppendIntelArr(testIntelArr, (Intel){
        true,
        ENTITY_HUMAN, ACTION_CAN, ENTITY_FLY,
        JUDGE_AUTO, STATE_UNKNOWN, true
    });

    return true;
}
void INTEL_Exit() {
    INTEL_ExitIntelArr();
    testIntelArr = INTEL_DeleteIntelArr(testIntelArr);
}


// RENEW ===============================================================================================================
bool INTEL_Renew() {
    INTEL_RenewIntelArr(testIntelArr);
    PERPH_SetKeyTrig(SDL_SCANCODE_TAB, (Trig){INTEL_ChangeMode, NULL, false});
    return true;
}


// DRAW ================================================================================================================
bool INTEL_Draw() {
    DEBUG_SendMessageL("%s: mode: %s\n", __func__, netMode ? "NET" : "SET");
    INTEL_DrawIntelArr(testIntelArr);
    return true;
}
