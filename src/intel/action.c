#include "action.h"
#include "entity.h"


Action actionSet[NUM_ACTIONS] = {
    [ACTION_NULL] = {.name = "unknown", .type = ACTION_TYPE_ONE_WAY},
    [ACTION_IS] = {.name = "is", .type = ACTION_TYPE_TWO_WAY},
    [ACTION_BELONG] = {.name = "belong", .type = ACTION_TYPE_ONE_WAY},
    [ACTION_WILL] = {.name = "will", .type = ACTION_TYPE_ONE_WAY},
    [ACTION_CAN] = {.name = "can", .type = ACTION_TYPE_ONE_WAY},
};
static const SDL_Color COLOR_DARK = {64, 64, 64, 192};
static const SDL_Color COLOR_LIGHT = {255, 255, 255, 255};
static const SDL_Color COLOR_T = {32, 128, 32, 192};
static const SDL_Color COLOR_F = {128, 32, 32, 192};
static const SDL_Color COLOR_AUTO = {255, 215, 0, 192};
static const struct {SDL_Color back, text;} CSET[INTEL_NUM_STATES] = {
    [INTEL_STATE_MANU_T] = {COLOR_T, COLOR_LIGHT},
    [INTEL_STATE_MANU_F] = {COLOR_F, COLOR_LIGHT},
    [INTEL_STATE_AUTO_UNKNOWN] = {COLOR_DARK, COLOR_AUTO},
    [INTEL_STATE_AUTO_T] = {COLOR_T, COLOR_AUTO},
    [INTEL_STATE_AUTO_F] = {COLOR_F, COLOR_AUTO},
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
bool INTEL_RenewAction() {
    for (int k = 0; k < intelNetNow->len; k++) {
        if (intelNetNow->intelSet[k].state == INTEL_STATE_AUTO_UNKNOWN)
            intelNetNow->intelSet[k].state = INTEL_GetAutoState(intelNetNow->intelSet[k]);
    }
    return true;
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
        if (actionSet[intel.action].type == ACTION_TYPE_ONE_WAY) {
            SDL_RenderDashedLine(renderer, A, B, 5, 5, time / 10);
        }
        else SDL_RenderLine(renderer, A.x, A.y, B.x, B.y);


        SDL_SetRenderColor(renderer, CSET[intel.state].back);
        SDL_RenderFillRect(renderer, &rect);

        SDL_SetTextureColorRGB(actionSet[intel.action].tex, CSET[intel.state].text);
        SDL_RenderTexture(renderer, actionSet[intel.action].tex, NULL, &rect);
    }
    return true;
}