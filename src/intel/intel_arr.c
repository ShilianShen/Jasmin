#include "intel_arr.h"
#include "entity.h"
#include "action.h"


IntelArr* intelArrNow = NULL;
static const SDL_Color JUDGE_COLOR[NUM_JUDGES] = {
    [JUDGE_AUTO] = {255, 215,   0, 192},
    [JUDGE_MANU] = {255, 255, 255, 255},
};
static const SDL_Color STATE_COLOR[NUM_STATES] = {
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
        const float w = (float)actionSet[intel.action].tex->w;
        const float h = (float)actionSet[intel.action].tex->h;
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

        SDL_SetTextureColorRGB(actionSet[intel.action].tex, text);
        SDL_RenderTexture(renderer, actionSet[intel.action].tex, NULL, &rect);
    }
    INTEL_DrawEntity();
    return true;
}
static bool INTEL_DrawIntelArr_Set() {
    // visible subject action object judge state
    int n = 1;
    for (int k = 0; k < intelArrNow->len; k++) {
        const Intel intel = intelArrNow->arr[k];
        if (intel.effective == false) continue;
        n++;
    }

    SetData strings[n];
    strings[0] = (SetData){"VISIBLE", "SUBJECT", "ACTION", "OBJECT", "JUDGE", "STATE"};
    for (int k = 0; k < intelArrNow->len; k++) {
        const Intel intel = intelArrNow->arr[k];
        if (intel.effective == false) continue;

        strings[k+1] = (SetData){
            intel.visible ? "1" : "0",
            entitySet[intel.subject].name,
            actionSet[intel.action].name,
            entitySet[intel.object].name,
            INTEL_JUDGE_STRING[intel.judge],
            INTEL_STATE_STRING[intel.state],
        };
    }

    float x = 0, y = 0, w = 400, h = 48;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < 6; j++) {
            const SDL_FPoint point = {x + (float)j * w, y + (float)i * h};
            SDL_RenderText(renderer, entityFont, strings[i].s[j], point, (SDL_Color){255, 255, 255, 255});
        }
    }
    return true;
}
bool INTEL_DrawIntelArr() {
    return netMode ? INTEL_DrawIntelArr_Net() : INTEL_DrawIntelArr_Set();
}