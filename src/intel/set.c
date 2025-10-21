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
static const float dx = 5, dy = 5;


// INIT & EXIT =========================================================================================================
bool INTEL_InitIntelSet() {
    font = TTF_OpenFont(SET_FONT);
    REQ_CONDITION(font != NULL, return false);

    struct {int num; const char** names; SDL_Texture** tex; float w;} NUMS[] = {
        {NUM_ENTITIES, ENTITY_NAMES, entityTex, 0},
        {NUM_ACTIONS, ACTION_NAMES, actionTex, 0},
        {NUM_JUDGES, JUDGE_NAMES, judgeTex, 0},
        {NUM_STATES, STATE_NAMES, stateTex, 0},
    };
    for (int I = 0; I < len_of(NUMS); I++) {
        const int num = NUMS[I].num;
        const char** names = NUMS[I].names;
        SDL_Texture** tex = NUMS[I].tex;

        for (int i = 0; i < num; i++) {
            tex[i] = TXT_LoadTexture(renderer, font, names[i], WHITE);
            REQ_CONDITION(tex[i] != NULL, return false);
            NUMS[I].w = SDL_max(NUMS[I].w, tex[i]->w);
        }
    }
    unitW[HEAD_SUBJECT] = unitW[HEAD_OBJECT] = NUMS[0].w;
    unitW[HEAD_ACTION] = NUMS[1].w;
    unitW[HEAD_JUDGE] = NUMS[2].w;
    unitW[HEAD_STATE] = NUMS[3].w;

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

    const struct {int num; SDL_Texture** tex;} NUMS[] = {
        {NUM_ENTITIES, entityTex},
        {NUM_ACTIONS, actionTex},
        {NUM_JUDGES, judgeTex},
        {NUM_STATES, stateTex},
    };
    for (int I = 0; I < len_of(NUMS); I++) {
        const int num = NUMS[I].num;
        SDL_Texture** tex = NUMS[I].tex;

        for (int i = 0; i < num; i++) {
            SDL_DestroyTexture(tex[i]);
            tex[i] = NULL;
        }
    }

    for (int i = 0; i < NUM_HEADS; i++) {
        SDL_DestroyTexture(headTex[i]);
        headTex[i] = NULL;
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

    for (int k = 0, j = 1; k < intelArrNow->len; k++) {
        const Intel intel = intelArrNow->arr[k];
        if (intel.effective == false) continue;

        tex[j][HEAD_VISIBLE] = visibleTex[intel.visible];
        tex[j][HEAD_SUBJECT] = entityTex[intel.subject];
        tex[j][HEAD_ACTION] = actionTex[intel.action];
        tex[j][HEAD_OBJECT] = entityTex[intel.object];
        tex[j][HEAD_JUDGE] = judgeTex[intel.judge];
        tex[j][HEAD_STATE] = stateTex[intel.state];
        n++;
    }

    SDL_SetRenderColor(renderer, (SDL_Color){64, 64, 64, 224});
    SDL_RenderFillRect(renderer, &bckRect);

    float y = bckRect.y;
    for (int k = 0, j = 0; k < intelArrNow->len; k++) {
        const Intel intel = intelArrNow->arr[k];
        if (intel.effective == false) continue;

        float x = bckRect.x;
        y += dy;
        for (int i = 0; i < NUM_HEADS; i++) {
            x += dx;
            if (tex[j][i] == NULL) continue;
            const SDL_FRect rect = {x, y, (float)tex[j][i]->w, (float)tex[j][i]->h};
            SDL_Color back = BLACK, text = WHITE;
            if (j != 0 && i == HEAD_JUDGE) text = JUDGE_COLOR[intel.judge];
            if (j != 0 && i == HEAD_STATE) back = STATE_COLOR[intel.state];

            SDL_SetRenderColor(renderer, back);
            SDL_RenderFillRect(renderer, &rect);

            SDL_SetTextureColorRGB(tex[j][i], text);
            SDL_RenderTexture(renderer, tex[j][i], NULL, &rect);

            x += unitW[i] + dx;
        }
        y += unitH + dy;
        j++;
    }

    return true;
}
