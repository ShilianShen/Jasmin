#include "intel_arr.h"
#include "set.h"
#include "net.h"
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
static bool INTEL_DrawIntelSet() {
    float x = 0;
    for (int i = 0; i < 6; i++) {
        const float dx = 20, dy = 10;
        float y = 0, w = 0;

        SDL_RenderTexture(renderer, setHeads[i], NULL, &(SDL_FRect){x, y, setHeads[i]->w, setHeads[i]->h});
        y += (float)setHeads[i]->h + dy;
        w = SDL_max(w, (float)setHeads[i]->w);

        for (int j = 0; j < intelArrNow->len; j++) {
            const Intel intel = intelArrNow->arr[j];
            if (intel.effective == false) continue;
            SDL_Texture* tex = NULL;
            switch (i) {
                case 0: tex = setHeads[i]; break;
                case 1: tex = entitySet[intel.subject].setTex; break;
                case 2: tex = actionSet[intel.action].setTex; break;
                case 3: tex = entitySet[intel.object].setTex; break;
                case 4: tex = setHeads[i]; break;
                case 5: tex = setHeads[i]; break;
                default: continue;
            }
            SDL_RenderTexture(renderer, tex, NULL, &(SDL_FRect){x, y, (float)tex->w, (float)tex->h});
            y += (float)tex->h + dy;
            w = SDL_max(w, (float)tex->w);
        }
        x += w + dx;
    }
    return true;
}
bool INTEL_DrawIntelArr() {
    return netMode ? INTEL_DrawIntelNet() : INTEL_DrawIntelSet();
}