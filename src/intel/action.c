#include "action.h"
#include "entity.h"


Action actionSet[NUM_ACTIONS] = {
    [ACTION_UNKNOWN] = {.name = "unknown"},
    [ACTION_IS] = {.name = "is"},
    [ACTION_BELONG] = {.name = "belong"},
    [ACTION_WILL] = {.name = "will"},
};
SDL_Color colorSet[INTEL_NUM_STATES] = {
    [INTEL_STATE_NULL] = {0, 0, 0, 0},
    [INTEL_STATE_SRC_TRUE] = {32, 128, 32, 128},
    [INTEL_STATE_SRC_FALSE] = {128, 32, 32, 128},
    [INTEL_STATE_UNKNOWN] = {128, 128, 128, 0},
};


bool INTEL_InitAction() {
    TTF_Font* font = TTF_OpenFont("../res/font/IBMPlexMono-Medium.ttf", 24);
    for (int i = 0; i < NUM_ACTIONS; i++) {
        const char* string = actionSet[i].name == NULL ? "????" : actionSet[i].name;
        actionSet[i].tex = TXT_LoadTexture(renderer, font, string, (SDL_Color){255, 255, 255, 255});
        REQ_CONDITION(actionSet[i].tex != NULL, {
            TTF_CloseFont(font); font = NULL;
            return false;
        });
    }
    TTF_CloseFont(font); font = NULL;
    return true;
}
void INTEL_ExitAction() {
    for (int i = 0; i < NUM_ACTIONS; i++) {
        SDL_DestroyTexture(actionSet[i].tex); actionSet[i].tex = NULL;
    }
}
bool INTEL_DrawAction(const IntelNet* intelNet) {
    for (int k = 0; k < intelNet->len; k++) {
        const Intel intel = intelNet->intelSet[k];
        if (intel.state == INTEL_STATE_NULL) continue;

        const int i = intel.subject, j = intel.object;
        const SDL_FPoint A = {
            windowRect.w / 2 + scale.x * entitySet[i].position.x,
            windowRect.h / 2 + scale.y * entitySet[i].position.y
        };
        const SDL_FPoint B = {
            windowRect.w / 2 + scale.x * entitySet[j].position.x,
            windowRect.h / 2 + scale.y * entitySet[j].position.y
        };
        const SDL_FPoint M = {(A.x + B.x) / 2, (A.y + B.y) / 2};
        const float w = (float)actionSet[intel.action].tex->w;
        const float h = (float)actionSet[intel.action].tex->h;
        const SDL_FRect rect = {M.x - w / 2, M.y - h / 2, w, h};
        SDL_SetRenderColor(renderer, colorSet[intel.state]);
        SDL_RenderLine(renderer, A.x, A.y, B.x, B.y);
        SDL_RenderFillRect(renderer, &rect);
        SDL_RenderTexture(renderer, actionSet[intel.action].tex, NULL, &rect);
    }
    return true;
}