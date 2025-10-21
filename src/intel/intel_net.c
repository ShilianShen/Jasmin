#include "intel_net.h"
#include "entity.h"
#include "action.h"
#include "intel_arr.h"


static SDL_Texture *entityTex[NUM_ENTITIES], *actionTex[NUM_ACTIONS];
static TTF_Font *entity_font = NULL, *action_font = NULL;


// GET & SET ===========================================================================================================
void INTEL_ResetIntelNet() {
    for (int i = 0; i < NUM_ENTITIES; i++) {
        entitySet[i].position = (SDL_FPoint){2 * SDL_randf() - 1, 2 * SDL_randf() - 1};
    }
}


// INIT & EXIT =========================================================================================================
bool INTEL_InitIntelNet() {
    entity_font = TTF_OpenFont(ENTITY_FONT);
    REQ_CONDITION(entity_font != NULL, return false);

    action_font = TTF_OpenFont(ACTION_FONT);
    REQ_CONDITION(action_font != NULL, return false);

    for (int i = 0; i < NUM_ENTITIES; i++) {
        entityTex[i] = TXT_LoadTexture(renderer, entity_font, ENTITY_NAMES[i], WHITE);
        REQ_CONDITION(entityTex[i] != NULL, return false);
    }

    for (int i = 0; i < NUM_ACTIONS; i++) {
        actionTex[i] = TXT_LoadTexture(renderer, action_font, ACTION_NAMES[i], WHITE);
        REQ_CONDITION(actionTex[i] != NULL, return false);
    }

    return true;
}
void INTEL_ExitIntelNet() {
    for (int i = 0; i < NUM_ENTITIES; i++) {
        SDL_DestroyTexture(entityTex[i]);
        entityTex[i] = NULL;
    }
    for (int i = 0; i < NUM_ACTIONS; i++) {
        SDL_DestroyTexture(actionTex[i]);
        actionTex[i] = NULL;
    }
    TTF_CloseFont(entity_font); entity_font = NULL;
    TTF_CloseFont(action_font); action_font = NULL;
}


// RENEW ===============================================================================================================
bool INTEL_RenewIntelNet() {
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
        const float w = (float)actionTex[intel.action]->w;
        const float h = (float)actionTex[intel.action]->h;
        intelArrNow->arr[k].rect = (SDL_FRect){M.x - w / 2, M.y - h / 2, w, h};
    }
    return true;
}


// DRAW ================================================================================================================
bool INTEL_DrawIntelNet() {
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

        SDL_SetTextureColorRGB(actionTex[intel.action], text);
        SDL_RenderTexture(renderer, actionTex[intel.action], NULL, &rect);
    }
    INTEL_DrawEntity();
    return true;
}

