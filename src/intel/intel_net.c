#include "intel_net.h"
#include "entity.h"
#include "action.h"


IntelNet* intelNetNow = NULL;
static const SDL_Color COLOR_DARK = {64, 64, 64, 192};
static const SDL_Color COLOR_LIGHT = {255, 255, 255, 255};
static const SDL_Color COLOR_T = {32, 128, 32, 192};
static const SDL_Color COLOR_F = {128, 32, 32, 192};
static const SDL_Color COLOR_AUTO = {255, 215, 0, 192};
static const struct {SDL_Color back, text;} CSET[NUM_STATES] = {
    [STATE_MANU_T] = {COLOR_T, COLOR_LIGHT},
    [STATE_MANU_F] = {COLOR_F, COLOR_LIGHT},
    [STATE_AUTO_U] = {COLOR_DARK, COLOR_AUTO},
    [STATE_AUTO_T] = {COLOR_T, COLOR_AUTO},
    [STATE_AUTO_F] = {COLOR_F, COLOR_AUTO},
};


bool INTEL_RenewIntelNet() {
    for (int k = 0; k < intelNetNow->len; k++) {
        const Intel intel = intelNetNow->intelSet[k];
        if (intel.subject == ENTITY_NULL && intel.action == ACTION_NULL && intel.object == ENTITY_NULL) {
            intelNetNow->intelSet[k].state = STATE_NULL;
        }
        if (intel.judge == JUDGE_AUTO) {
            intelNetNow->intelSet[k].state = INTEL_GetAutoState(intel);
        }
        const int i = intel.subject, j = intel.object;
        const SDL_FPoint A = INTEL_GetScaledPos(entitySet[i].position);
        const SDL_FPoint B = INTEL_GetScaledPos(entitySet[j].position);
        const SDL_FPoint M = {(A.x + B.x) / 2, (A.y + B.y) / 2};
        const float w = (float)actionSet[intel.action].tex->w;
        const float h = (float)actionSet[intel.action].tex->h;
        intelNetNow->intelSet[k].rect = (SDL_FRect){M.x - w / 2, M.y - h / 2, w, h};
    }
    return true;
}
bool INTEL_DrawIntelNet() {
    const float time = (float)SDL_GetTicks();
    for (int k = 0; k < intelNetNow->len; k++) {
        const Intel intel = intelNetNow->intelSet[k];
        if (intel.state == STATE_NULL) continue;

        const int i = intel.subject, j = intel.object;
        const SDL_FPoint A = INTEL_GetScaledPos(entitySet[i].position);
        const SDL_FPoint B = INTEL_GetScaledPos(entitySet[j].position);
        const SDL_FRect rect = intelNetNow->intelSet[k].rect;

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        if (actionSet[intel.action].type == ACTION_TYPE_ONE_WAY) {
            SDL_RenderDashedLine(renderer, A, B, 5, 5, time / 10);
        }
        else SDL_RenderLine(renderer, A.x, A.y, B.x, B.y);

        if (PERPH_GetMouseLeftInRect(rect)) {
            DEBUG_SendMessageR("%s: %s.\n", __func__, INTEL_GetStrIntel(intel));
        }
        SDL_SetRenderColor(renderer, PERPH_GetMouseLeftInRect(rect) ? CSET[intel.state].text : CSET[intel.state].back);
        SDL_RenderFillRect(renderer, &rect);

        SDL_SetTextureColorRGB(actionSet[intel.action].tex, PERPH_GetMouseLeftInRect(rect) ? CSET[intel.state].back : CSET[intel.state].text);
        SDL_RenderTexture(renderer, actionSet[intel.action].tex, NULL, &rect);
    }
    return true;
}