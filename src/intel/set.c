#include "set.h"
#include "entity.h"
#include "action.h"
#include "intel_arr.h"


static TTF_Font *font = NULL;
static enum {HEAD_VISIBLE, HEAD_SUBJECT, HEAD_ACTION, HEAD_OBJECT, HEAD_JUDGE, HEAD_STATE, NUM_HEADS} Head;
static const char* HEAD_SET[NUM_HEADS] = {
    [HEAD_VISIBLE] = "VISIBLE",
    [HEAD_SUBJECT] = "SUBJECT",
    [HEAD_ACTION] = "ACTION",
    [HEAD_OBJECT] = "OBJECT",
    [HEAD_JUDGE] = "JUDGE",
    [HEAD_STATE] = "STATE"
};
static SDL_Texture *entityTex[NUM_ENTITIES], *actionTex[NUM_ACTIONS];
static SDL_Texture *judgeTex[NUM_JUDGES], *stateTex[NUM_STATES], *headTex[NUM_HEADS];
static SDL_Texture *visibleTex[2];
static float unitW[NUM_HEADS] = {0}, unitH = 0;
static const float dx = 10, dy = 10;


// INIT & EXIT =========================================================================================================
bool INTEL_InitIntelSet() {
    font = TTF_OpenFont(SET_FONT);
    REQ_CONDITION(font != NULL, return false);

    for (int i = 0; i < NUM_ENTITIES; i++) {
        entityTex[i] = TXT_LoadTexture(renderer, font, ENTITY_NAMES[i], WHITE);
        REQ_CONDITION(entityTex[i] != NULL, return false);
        unitW[HEAD_SUBJECT] = unitW[HEAD_OBJECT] = SDL_max(unitW[HEAD_OBJECT], entityTex[i]->w);
    }
    for (int i = 0; i < NUM_ACTIONS; i++) {
        actionTex[i] = TXT_LoadTexture(renderer, font, ACTION_NAMES[i], WHITE);
        REQ_CONDITION(actionTex[i] != NULL, return false);
        unitW[HEAD_ACTION] = SDL_max(unitW[HEAD_ACTION], actionTex[i]->w);
    }
    for (int i = 0; i < NUM_JUDGES; i++) {
        judgeTex[i] = TXT_LoadTexture(renderer, font, INTEL_JUDGE_STRING[i], WHITE);
        REQ_CONDITION(judgeTex[i] != NULL, return false);
        unitW[HEAD_JUDGE] = SDL_max( unitW[HEAD_JUDGE], judgeTex[i]->w);
    }
    for (int i = 0; i < NUM_STATES; i++) {
        stateTex[i] = TXT_LoadTexture(renderer, font, INTEL_STATE_STRING[i], WHITE);
        REQ_CONDITION(stateTex[i] != NULL, return false);
        unitW[HEAD_STATE] = SDL_max(unitW[HEAD_STATE], stateTex[i]->w);
    }

    visibleTex[0] = TXT_LoadTexture(renderer, font, "1", WHITE);
    visibleTex[1] = TXT_LoadTexture(renderer, font, "0", WHITE);
    REQ_CONDITION(visibleTex[0] != NULL, return false);
    REQ_CONDITION(visibleTex[1] != NULL, return false);
    unitW[HEAD_VISIBLE] = (float)SDL_max(visibleTex[0]->w, visibleTex[1]->w);

    for (int i = 0; i < NUM_HEADS; i++) {
        headTex[i] = TXT_LoadTexture(renderer, font, HEAD_SET[i], WHITE);
        REQ_CONDITION(headTex[i] != NULL, return false);
        unitW[i] = SDL_max(unitW[i], headTex[i]->w);
    }
    unitH = (float)TTF_GetFontHeight(font);

    return true;
}
void INTEL_ExitIntelSet() {
    TTF_CloseFont(font); font = NULL;
    for (int i = 0; i < NUM_ENTITIES; i++) {
        SDL_DestroyTexture(entityTex[i]); entityTex[i] = NULL;
    }
    for (int i = 0; i < NUM_ACTIONS; i++) {
        SDL_DestroyTexture(actionTex[i]); actionTex[i] = NULL;
    }
    for (int i = 0; i < NUM_HEADS; i++) {
        SDL_DestroyTexture(headTex[i]); headTex[i] = NULL;
    }
    for (int i = 0; i < NUM_JUDGES; i++) {
        SDL_DestroyTexture(judgeTex[i]); judgeTex[i] = NULL;
    }
    for (int i = 0; i < NUM_STATES; i++) {
        SDL_DestroyTexture(stateTex[i]); stateTex[i] = NULL;
    }

    SDL_DestroyTexture(visibleTex[0]); visibleTex[0] = NULL;
    SDL_DestroyTexture(visibleTex[1]); visibleTex[1] = NULL;
}


// RENEW ===============================================================================================================
bool INTEL_RenewIntelSet() {
    return true;
}


// DRAW ================================================================================================================
bool INTEL_DrawIntelSet() {
    int n = 1;
    for (int k = 0; k < intelArrNow->len; k++) {
        const Intel intel = intelArrNow->arr[k];
        if (intel.effective == false) continue;
        n++;
    }

    SDL_Texture* tex[n][NUM_HEADS];
    for (int j = 0; j < n; j++) {
        for (int i = 0; i < 6; i++) {
            tex[j][i] = NULL;
        }
    }
    for (int i = 0; i < NUM_HEADS; i++) {
        tex[0][i] = headTex[i];
    }

    SDL_FRect bckRect = {0, 0, 0, (float)n * (unitH + 2 * dy)};
    for (int i = 0; i < NUM_HEADS; i++) bckRect.w += unitW[i] + 2 * dx;
    bckRect.x = windowRect.x + (windowRect.w - bckRect.w) / 2;
    bckRect.y = windowRect.y + (windowRect.h - bckRect.h) / 2;

    n = 1;
    for (int k = 0; k < intelArrNow->len; k++) {
        const Intel intel = intelArrNow->arr[k];
        if (intel.effective == false) continue;

        tex[n][HEAD_VISIBLE] = visibleTex[intel.visible];
        tex[n][HEAD_SUBJECT] = entityTex[intel.subject];
        tex[n][HEAD_ACTION] = actionTex[intel.action];
        tex[n][HEAD_OBJECT] = entityTex[intel.object];
        tex[n][HEAD_JUDGE] = judgeTex[intel.judge];
        tex[n][HEAD_STATE] = stateTex[intel.state];
        n++;
    }

    SDL_SetRenderColor(renderer, (SDL_Color){250, 250, 25, 55});
    SDL_RenderFillRect(renderer, &bckRect);
    float y = bckRect.y;
    for (int j = 0; j < n; j++) {
        float x = bckRect.x;
        y += dy;
        for (int i = 0; i < NUM_HEADS; i++) {
            x += dx;
            if (tex[j][i] == NULL) continue;
            SDL_FRect rect = {x, y, tex[j][i]->w, tex[j][i]->h};
            SDL_RenderTexture(renderer, tex[j][i], NULL, &rect);
            x += unitW[i] + dx;
        }
        y += unitH + dy;
    }

    return true;
}
