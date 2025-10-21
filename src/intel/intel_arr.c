#include "intel_arr.h"
#include "intel_set.h"
#include "intel_net.h"
#include "entity.h"
#include "action.h"


IntelArr* intelArrNow = NULL;
const SDL_Color JUDGE_COLOR[NUM_JUDGES] = {
    [JUDGE_AUTO] = {255, 215,   0, 192},
    [JUDGE_MANU] = {255, 255, 255, 255},
};
const SDL_Color STATE_COLOR[NUM_STATES] = {
    [STATE_UNKNOWN] = { 64,  64,  64, 192},
    [STATE_TRUE] = { 32, 128,  32, 192},
    [STATE_FALSE] = {128,  32,  32, 192},
    [STATE_PARADOX] = {  0,   0, 255,   0},
};


// RENEW ===============================================================================================================
bool INTEL_RenewIntelArr() {
    if (netMode) {
        INTEL_RenewEntity();
        INTEL_RenewIntelNet();
    }
    return true;
}


// DRAW ================================================================================================================
typedef struct {const char* s[6];} SetData;
bool INTEL_DrawIntelArr() {
    return netMode ? INTEL_DrawIntelNet() : INTEL_DrawIntelSet();
}