#include "intel_set.h"
#include "entity.h"
#include "action.h"
#include "intel_arr.h"


static TTF_Font *font = NULL;
enum {HEAD_VISIBLE, HEAD_SUBJECT, HEAD_ACTION, HEAD_OBJECT, HEAD_JUDGE, HEAD_STATE, NUM_HEADS};
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
static const float dx = 10, dy = 5;
static const SDL_Color backColor = {64, 64, 64, 128};


// GET & SET ===========================================================================================================



// TRIG ================================================================================================================
void TRIG_ChangeVisible(const void* para) {

}


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
static bool INTEL_DrawIntelSet_Head(const SDL_FRect bckRect) {
    float y = bckRect.y + dy;
    float x = bckRect.x;
    for (int i = 0; i < NUM_HEADS; i++) {
        x += dx;
        SDL_FRect rect = {x, y, (float)headTex[i]->w, (float)headTex[i]->h};
        SDL_RenderTexture(renderer, headTex[i], NULL, &rect);
        x += unitW[i] + dx;
    }
    y += unitH + dy;
    SDL_SetRenderColor(renderer, WHITE);
    SDL_RenderLine(renderer, bckRect.x, y, x, y);
    return true;
}
static bool INTEL_DrawIntelSet_Body(const int N, const Intel intelSet[N], const SDL_FRect bckRect) {
    for (int i = 0; i < N; i++) {
        float y = bckRect.y + (float)(i + 1) * (unitH + 2 * dy) + dy;
        float x = bckRect.x;
        const Intel intel = intelSet[i];

        for (int j = 0; j < NUM_HEADS; j++) {
            x += dx;
            SDL_Color back = EMPTY, text = WHITE;
            SDL_Texture* tx = NULL;
            switch (j) {
                case HEAD_VISIBLE: tx = visibleTex[intel.visible]; break;
                case HEAD_SUBJECT: tx = entityTex[intel.subject]; break;
                case HEAD_ACTION: tx = actionTex[intel.action]; break;
                case HEAD_OBJECT: tx = entityTex[intel.object]; break;
                case HEAD_JUDGE: {
                    back = backColor;
                    tx = judgeTex[intel.judge];
                    break;
                }
                case HEAD_STATE: {
                    tx = stateTex[intel.state];
                    back = STATE_COLORS[intel.state];
                    text = JUDGE_COLORS[intel.judge];
                    break;
                }
                default: continue;
            }
            const SDL_FRect rect = {x, y, (float)tx->w, (float)tx->h};
            if (PERPH_GetMouseInRect(rect)) {
                SDL_Color temp = text;
                text = back;
                back = temp;
            }
            SDL_SetRenderColor(renderer, back);
            SDL_RenderFillRect(renderer, &rect);
            SDL_SetTextureColorRGB(tx, text);
            SDL_RenderTexture(renderer, tx, NULL, &rect);
            x += unitW[j] + dx;
        }
    }
    return true;
}
bool INTEL_DrawIntelSet() {
    int N = 0;
    for (int k = 0; k < intelArrNow->len; k++) {
        const Intel intel = intelArrNow->arr[k];
        if (intel.effective == false) continue;
        N++;
    }
    Intel intelSet[N];
    for (int k = 0, i = 0; k < intelArrNow->len; k++) {
        const Intel intel = intelArrNow->arr[k];
        if (intel.effective == false) continue;
        intelSet[i] = intel;
        i++;
    }

    SDL_FRect bckRect = {0, 0, 0, (float)(N + 1) * (unitH + 2 * dy)};
    for (int i = 0; i < NUM_HEADS; i++) bckRect.w += unitW[i] + 2 * dx;
    bckRect.x = windowRect.x + (windowRect.w - bckRect.w) / 2;
    bckRect.y = windowRect.y + (windowRect.h - bckRect.h) / 2;

    SDL_SetRenderColor(renderer, backColor);
    SDL_RenderFillRect(renderer, &bckRect);
    INTEL_DrawIntelSet_Head(bckRect);
    INTEL_DrawIntelSet_Body(N, intelSet, bckRect);

    return true;
}
