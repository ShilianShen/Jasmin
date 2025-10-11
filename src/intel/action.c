#include "action.h"
#include "entity.h"


Action actionSet[NUM_ACTIONS] = {
    [ACTION_UNKNOWN] = {.name = "unknown", .type = ACTION_TYPE_ONE_WAY},
    [ACTION_IS] = {.name = "is", .type = ACTION_TYPE_TWO_WAY},
    [ACTION_BELONG] = {.name = "belong", .type = ACTION_TYPE_ONE_WAY},
    [ACTION_WILL] = {.name = "will", .type = ACTION_TYPE_ONE_WAY},
};
static const SDL_Color COLOR_SET[INTEL_NUM_STATES] = {
    [INTEL_STATE_NULL] = {0, 0, 0, 0},
    [INTEL_STATE_SRC_TRUE] = {32, 128, 32, 192},
    [INTEL_STATE_SRC_FALSE] = {128, 32, 32, 192},
    [INTEL_STATE_UNKNOWN] = {64, 64, 64, 192},
    [INTEL_STATE_AUTO_UNKNOWN] = {32, 32, 128, 192},
};
static const SDL_Color FONT_COLOR = {255, 255, 255, 255};
static const char* FONT_PATH = "../res/font/IBMPlexMono-Medium.ttf";
static const float FONT_SIZE = 24;


bool INTEL_InitAction() {
    TTF_Font* font = TTF_OpenFont(FONT_PATH, FONT_SIZE);
    REQ_CONDITION(font != NULL, return false);
    for (int i = 0; i < NUM_ACTIONS; i++) {
        const char* string = actionSet[i].name == NULL ? "????" : actionSet[i].name;
        actionSet[i].tex = TXT_LoadTexture(renderer, font, string, FONT_COLOR);
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
bool INTEL_DrawAction() {
    const float time = (float)SDL_GetTicks();
    for (int k = 0; k < intelNetNow->len; k++) {
        const Intel intel = intelNetNow->intelSet[k];
        if (intel.state == INTEL_STATE_NULL) continue;

        const int i = intel.subject, j = intel.object;
        const SDL_FPoint A = INTEL_GetScaledPos(entitySet[i].position);
        const SDL_FPoint B = INTEL_GetScaledPos(entitySet[j].position);
        const SDL_FPoint M = {(A.x + B.x) / 2, (A.y + B.y) / 2};
        const float w = (float)actionSet[intel.action].tex->w;
        const float h = (float)actionSet[intel.action].tex->h;
        const SDL_FRect rect = {M.x - w / 2, M.y - h / 2, w, h};

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        switch (intel.action) {
            case ACTION_IS: {
                SDL_RenderParallelLine(renderer, A, B, 8);
                break;
            }
            case ACTION_BELONG: {
                SDL_RenderDashedLine(renderer, A, M, 5, 5, time / 10);
                SDL_RenderParallelLine(renderer, M, B, 8);
                break;
            }
            default: {
                switch (actionSet[intel.action].type) {
                    case ACTION_TYPE_ONE_WAY: {
                        SDL_RenderDashedLine(renderer, A, B, 5, 5, time / 10);
                        break;
                    }
                    default: SDL_RenderLine(renderer, A.x, A.y, B.x, B.y);
                }
            }
        }

        const IntelState state = intel.state == INTEL_STATE_AUTO_UNKNOWN ? INTEL_GetAutoState(intel) : intel.state;
        SDL_SetRenderColor(renderer, COLOR_SET[state]);
        SDL_RenderFillRect(renderer, &rect);
        SDL_RenderTexture(renderer, actionSet[intel.action].tex, NULL, &rect);
    }
    return true;
}