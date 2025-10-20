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
static bool INTEL_RenewIntelArr_Net() {
    for (int k = 0; k < intelArrNow->len; k++) {
        const Intel intel = intelArrNow->arr[k];
        if (intel.subject == ENTITY_NULL && intel.action == ACTION_NULL && intel.object == ENTITY_NULL) {
            // intelArrNow->arr[k].effective == false;
        }
        if (intel.judge == JUDGE_AUTO) {
            intelArrNow->arr[k].state = INTEL_GetAutoState(intel);
        }
        const int i = intel.subject, j = intel.object;
        const SDL_FPoint A = INTEL_GetScaledPos(entitySet[i].position);
        const SDL_FPoint B = INTEL_GetScaledPos(entitySet[j].position);
        const SDL_FPoint M = {(A.x + B.x) / 2, (A.y + B.y) / 2};
        const float w = (float)actionSet[intel.action].netTex->w;
        const float h = (float)actionSet[intel.action].netTex->h;
        intelArrNow->arr[k].rect = (SDL_FRect){M.x - w / 2, M.y - h / 2, w, h};
    }
    return true;
}
bool INTEL_RenewIntelArr() {
    if (netMode) {
        INTEL_RenewEntity();
        INTEL_RenewIntelArr_Net();
    }
    return true;
}


// DRAW ================================================================================================================
typedef struct {const char* s[6];} SetData;
static bool INTEL_DrawIntelArr_Net() {
    const float time = (float)SDL_GetTicks();
    for (int k = 0; k < intelArrNow->len; k++) {
        const Intel intel = intelArrNow->arr[k];
        if (intel.effective == false) continue;

        const int i = intel.subject, j = intel.object;
        const SDL_FPoint A = INTEL_GetScaledPos(entitySet[i].position);
        const SDL_FPoint B = INTEL_GetScaledPos(entitySet[j].position);
        const SDL_FRect rect = intelArrNow->arr[k].rect;

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        if (actionSet[intel.action].type == ACTION_TYPE_ONE_WAY) {
            SDL_RenderDashedLine(renderer, A, B, 5, 5, time / 10);
        }
        else SDL_RenderLine(renderer, A.x, A.y, B.x, B.y);

        if (PERPH_GetMouseLeftInRect(rect)) {
            DEBUG_SendMessageR("%s: %s.\n", __func__, INTEL_GetStrIntel(intel));
        }

        SDL_Color text = JUDGE_COLOR[intel.judge], back = STATE_COLOR[intel.state];
        if (PERPH_GetMouseLeftInRect(rect)) {
            const SDL_Color temp = text;
            text = back;
            back = temp;
        }

        SDL_SetRenderColor(renderer, back);
        SDL_RenderFillRect(renderer, &rect);

        SDL_SetTextureColorRGB(actionSet[intel.action].netTex, text);
        SDL_RenderTexture(renderer, actionSet[intel.action].netTex, NULL, &rect);
    }
    INTEL_DrawEntity();
    return true;
}
static bool INTEL_DrawIntelArr_Set() {
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
    return netMode ? INTEL_DrawIntelArr_Net() : INTEL_DrawIntelArr_Set();
}