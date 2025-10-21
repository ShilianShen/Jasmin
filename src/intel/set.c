#include "set.h"
#include "entity.h"
#include "action.h"
#include "intel_arr.h"


static SDL_Texture *entityTex[NUM_ENTITIES], *actionTex[NUM_ACTIONS], *judgeTex[NUM_JUDGES], *stateTex[NUM_STATES];
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
static SDL_Texture *headTex[NUM_HEADS];


// INIT & EXIT =========================================================================================================
bool INTEL_InitIntelSet() {
    font = TTF_OpenFont(SET_FONT);
    REQ_CONDITION(font != NULL, return false);

    for (int i = 0; i < NUM_ENTITIES; i++) {
        entityTex[i] = TXT_LoadTexture(renderer, font, entitySet[i].name, (SDL_Color){255, 255, 255, 255});
        REQ_CONDITION(entityTex[i] != NULL, return false);
    }
    for (int i = 0; i < NUM_ACTIONS; i++) {
        actionTex[i] = TXT_LoadTexture(renderer, font, actionSet[i].name, (SDL_Color){255, 255, 255, 255});
        REQ_CONDITION(actionTex[i] != NULL, return false);
    }
    for (int i = 0; i < NUM_JUDGES; i++) {
        judgeTex[i] = TXT_LoadTexture(renderer, font, INTEL_JUDGE_STRING[i], (SDL_Color){255, 255, 255, 255});
        REQ_CONDITION(judgeTex[i] != NULL, return false);
    }
    for (int i = 0; i < NUM_STATES; i++) {
        stateTex[i] = TXT_LoadTexture(renderer, font, INTEL_STATE_STRING[i], (SDL_Color){255, 255, 255, 255});
        REQ_CONDITION(stateTex[i] != NULL, return false);
    }
    for (int i = 0; i < NUM_HEADS; i++) {
        headTex[i] = TXT_LoadTexture(renderer, font, HEAD_SET[i], (SDL_Color){255, 255, 255, 255});
        REQ_CONDITION(headTex[i] != NULL, return false);
    }

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

    n = 1;
    for (int k = 0; k < intelArrNow->len; k++) {
        const Intel intel = intelArrNow->arr[k];
        if (intel.effective == false) continue;

        tex[n][HEAD_SUBJECT] = entityTex[intel.subject];
        tex[n][HEAD_ACTION] = actionTex[intel.action];
        tex[n][HEAD_OBJECT] = entityTex[intel.object];
        tex[n][HEAD_JUDGE] = judgeTex[intel.judge];
        tex[n][HEAD_STATE] = stateTex[intel.state];
        n++;
    }

    float y = 0;
    for (int j = 0; j < n; j++) {
        float x = 0, h = 0;
        for (int i = 0; i < NUM_HEADS; i++) {
            if (tex[j][i] == NULL) continue;
            SDL_FRect rect = {x, y, tex[j][i]->w, tex[j][i]->h};
            SDL_RenderTexture(renderer, tex[j][i], NULL, &rect);
            x += tex[j][i]->w;
            h = SDL_max(h, tex[j][i]->h);
        }
        y += h;
    }

    return true;
}
