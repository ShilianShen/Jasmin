#include "intel_set.h"
#include "entity.h"
#include "action.h"
#include "intel_arr.h"


typedef enum {TYPE_VISIBLE, TYPE_ENTITY, TYPE_ACTION, TYPE_JUDGE, TYPE_STATE, NUM_TYPES} SetType;
static struct {const int num; const char** names; SDL_Texture** tex; float w;} setInfo[NUM_TYPES] = {
    [TYPE_VISIBLE] = {2, (const char*[]){"FALSE", "TRUE"}, (SDL_Texture*[2]){}, 0},
    [TYPE_ENTITY] = {NUM_ENTITIES, ENTITY_NAMES, (SDL_Texture*[NUM_ENTITIES]){}, 0},
    [TYPE_ACTION] = {NUM_ACTIONS, ACTION_NAMES, (SDL_Texture*[NUM_ACTIONS]){}, 0},
    [TYPE_JUDGE] = {NUM_JUDGES, JUDGE_NAMES, (SDL_Texture*[NUM_JUDGES]){}, 0},
    [TYPE_STATE] = {NUM_STATES, STATE_NAMES, (SDL_Texture*[NUM_STATES]){}, 0},
};


typedef enum {HEAD_VISIBLE, HEAD_SUBJECT, HEAD_ACTION, HEAD_OBJECT, HEAD_JUDGE, HEAD_STATE, NUM_HEADS} SetHead;
static struct {const char* name; SetType type; SDL_Texture* tex;} HEAD_SET[NUM_HEADS] = {
    [HEAD_VISIBLE] = {"VISIBLE", TYPE_VISIBLE, NULL},
    [HEAD_SUBJECT] = {"SUBJECT", TYPE_ENTITY, NULL},
    [HEAD_ACTION] = {"ACTION", TYPE_ACTION, NULL},
    [HEAD_OBJECT] = {"OBJECT", TYPE_ENTITY, NULL},
    [HEAD_JUDGE] = {"JUDGE", TYPE_JUDGE, NULL},
    [HEAD_STATE] = {"STATE", TYPE_STATE, NULL},
};
// static SDL_Texture *headTex[NUM_HEADS];


static float unitW[NUM_HEADS] = {0}, unitH = 0;
static const float dx = 10, dy = 5;
static const SDL_Color BACK_C = {64, 64, 64, 128};
static SDL_FRect headRect[NUM_HEADS];


static const int LEN_BUFFER = 100;
static struct {Intel* intel; SDL_FRect rect[NUM_HEADS];} buffer[LEN_BUFFER];
static int bufferHead = 0;


// GET & SET ===========================================================================================================
SDL_FRect INTEL_GetIntelSetBckRect() {
    SDL_FRect bckRect = {0, 0, 0, (float)(bufferHead + 1) * (unitH + 2 * dy)};
    for (int i = 0; i < NUM_HEADS; i++) bckRect.w += unitW[i] + 2 * dx;
    bckRect.x = windowRect.x + (windowRect.w - bckRect.w) / 2;
    bckRect.y = windowRect.y + (windowRect.h - bckRect.h) / 2;
    return bckRect;
}


// TRIG ================================================================================================================
void Intel_TrigChangeVisible(void* para) {
    Intel* intel = para;
    intel->visible = !intel->visible;
}
void Intel_TrigChangeJudge(void* para) {
    Intel* intel = para;
    intel->judge = (intel->judge + 1) % NUM_JUDGES;
    intel->state = STATE_UNKNOWN;
}
void Intel_TrigChangeState(void* para) {
    Intel* intel = para;
    if (intel->judge != JUDGE_MANU) return;
    intel->state = (intel->state + 1) % NUM_STATES;
}
static const TrigFunc HEAD_TRIG[NUM_HEADS] = {
    [HEAD_VISIBLE] = Intel_TrigChangeVisible,
    [HEAD_JUDGE] = Intel_TrigChangeJudge,
    [HEAD_STATE] = Intel_TrigChangeState,
};


// INIT & EXIT =========================================================================================================
static bool INTEL_InitIntelSet_RK(TTF_Font* font) {
    REQ_CONDITION(font != NULL, return false);

    for (int I = 0; I < NUM_TYPES; I++) {
        const int num = setInfo[I].num;
        const char** names = setInfo[I].names;
        SDL_Texture** tex = setInfo[I].tex;

        for (int i = 0; i < num; i++) {
            tex[i] = TXT_LoadTexture(renderer, font, names[i], WHITE);
            REQ_CONDITION(tex[i] != NULL, return false);
            setInfo[I].w = SDL_max(setInfo[I].w, tex[i]->w);
        }
    }
    unitW[HEAD_SUBJECT] = setInfo[TYPE_ENTITY].w;
    unitW[HEAD_ACTION] = setInfo[TYPE_ACTION].w;
    unitW[HEAD_OBJECT] = setInfo[TYPE_ENTITY].w;
    unitW[HEAD_JUDGE] = setInfo[TYPE_JUDGE].w;
    unitW[HEAD_STATE] = setInfo[TYPE_STATE].w;
    unitW[HEAD_VISIBLE] = setInfo[TYPE_VISIBLE].w;

    for (int i = 0; i < NUM_HEADS; i++) {
        HEAD_SET[i].tex = TXT_LoadTexture(renderer, font, HEAD_SET[i].name, WHITE);
        REQ_CONDITION(HEAD_SET[i].tex != NULL, return false);
        unitW[i] = SDL_max(unitW[i], HEAD_SET[i].tex->w);
    }
    unitH = (float)TTF_GetFontHeight(font);
    return true;
}
bool INTEL_InitIntelSet() {
    TTF_Font *font = TTF_OpenFont(SET_FONT);

    const bool rk = INTEL_InitIntelSet_RK(font);
    TTF_CloseFont(font); font = NULL;

    REQ_CONDITION(rk != false, return false);
    return true;
}
void INTEL_ExitIntelSet() {
    for (int I = 0; I < NUM_TYPES; I++) {
        const int num = setInfo[I].num;
        SDL_Texture** tex = setInfo[I].tex;

        for (int i = 0; i < num; i++) {
            SDL_DestroyTexture(tex[i]);
            tex[i] = NULL;
        }
    }

    for (int i = 0; i < NUM_HEADS; i++) {
        SDL_DestroyTexture(HEAD_SET[i].tex);
        HEAD_SET[i].tex = NULL;
    }
}


// RENEW ===============================================================================================================
static bool INTEL_RenewIntelSet_Buffer(IntelArr* intelArr) {
    bufferHead = 0;
    for (int k = 0; k < intelArr->len && bufferHead < LEN_BUFFER; k++) {
        Intel* intel = &intelArr->arr[k];
        if (intel->effective == false) continue;
        buffer[bufferHead].intel = intel;
        bufferHead++;
    }
    return true;
}
static bool INTEL_RenewIntelSet_Head(const SDL_FRect bckRect) {
    const float y = bckRect.y + dy;
    float x = bckRect.x;
    for (int i = 0; i < NUM_HEADS; i++) {
        x += dx;
        headRect[i] = (SDL_FRect){x, y, (float)HEAD_SET[i].tex->w, (float)HEAD_SET[i].tex->h};
        x += unitW[i] + dx;
    }
    return true;
}
static bool INTEL_RenewIntelSet_Body(const SDL_FRect bckRect) {
    for (int i = 0; i < bufferHead; i++) {
        const float y = bckRect.y + (float)(i + 1) * (unitH + 2 * dy) + dy;
        float x = bckRect.x;
        const Intel intel = *buffer[i].intel;

        for (int j = 0; j < NUM_HEADS; j++) {
            x += dx;
            const SDL_Texture* tx = NULL;
            switch (j) {
                case HEAD_VISIBLE: tx = setInfo[TYPE_VISIBLE].tex[intel.visible]; break;
                case HEAD_SUBJECT: tx = setInfo[TYPE_ENTITY].tex[intel.subject]; break;
                case HEAD_ACTION: tx = setInfo[TYPE_ACTION].tex[intel.action]; break;
                case HEAD_OBJECT: tx = setInfo[TYPE_ENTITY].tex[intel.object]; break;
                case HEAD_JUDGE: tx = setInfo[TYPE_JUDGE].tex[intel.judge]; break;
                case HEAD_STATE: tx = setInfo[TYPE_STATE].tex[intel.state]; break;
                default: continue;
            }
            buffer[i].rect[j] = (SDL_FRect){x, y, (float)tx->w, (float)tx->h};
            x += unitW[j] + dx;
        }
    }
    return true;
}
static bool INTEL_RenewIntelSet_Trig() {
    for (int i = 0; i < bufferHead; i++) {
        Intel* intel = buffer[i].intel;
        for (int j = 0; j < NUM_HEADS; j++) {
            const SDL_FRect rect = buffer[i].rect[j];
            if (PERPH_GetMouseInRect(rect) == false) continue;
            if (PERPH_GetMouseLeftInRect(rect) == false) continue;
            PERPH_SetMouseLeftTrig((Trig){HEAD_TRIG[j], intel, false});
        }
    }
    return true;
}
bool INTEL_RenewIntelSet(IntelArr* intelArr) {
    REQ_CONDITION(INTEL_RenewIntelSet_Buffer(intelArr), return false);

    const SDL_FRect bckRect = INTEL_GetIntelSetBckRect();

    REQ_CONDITION(INTEL_RenewIntelSet_Head(bckRect), return false);
    REQ_CONDITION(INTEL_RenewIntelSet_Body(bckRect), return false);
    REQ_CONDITION(INTEL_RenewIntelSet_Trig(), return false);
    return true;
}


// DRAW ================================================================================================================
static bool INTEL_DrawIntelSet_Head() {
    for (int i = 0; i < NUM_HEADS; i++) {
        SDL_FRect rect = headRect[i];
        SDL_RenderTexture(renderer, HEAD_SET[i].tex, NULL, &rect);
    }
    return true;
}
static bool INTEL_DrawIntelSet_Body() {
    for (int i = 0; i < bufferHead; i++) {
        const Intel intel = *buffer[i].intel;
        for (int j = 0; j < NUM_HEADS; j++) {
            SDL_Color back = EMPTY, text = WHITE;
            SDL_Texture* tx = NULL;
            switch (j) {
                case HEAD_VISIBLE: tx = setInfo[TYPE_VISIBLE].tex[intel.visible]; break;
                case HEAD_SUBJECT: tx = setInfo[TYPE_ENTITY].tex[intel.subject]; break;
                case HEAD_ACTION: tx = setInfo[TYPE_ACTION].tex[intel.action]; break;
                case HEAD_OBJECT: tx = setInfo[TYPE_ENTITY].tex[intel.object]; break;
                case HEAD_JUDGE: {
                    back = BACK_C;
                    tx = setInfo[TYPE_JUDGE].tex[intel.judge];
                    break;
                }
                case HEAD_STATE: {
                    tx = setInfo[TYPE_STATE].tex[intel.state];
                    back = STATE_COLORS[intel.state];
                    text = JUDGE_COLORS[intel.judge];
                    break;
                }
                default: continue;
            }
            const SDL_FRect rect = buffer[i].rect[j];
            if (PERPH_GetMouseInRect(rect) && PERPH_GetMouseLeftInRect(rect)) {
                const SDL_Color temp = text;
                text = back;
                back = temp;
            }
            SDL_SetRenderColor(renderer, back);
            SDL_RenderFillRect(renderer, &rect);
            SDL_SetTextureColorRGB(tx, text);
            SDL_RenderTexture(renderer, tx, NULL, &rect);
        }
    }
    return true;
}
bool INTEL_DrawIntelSet() {
    SDL_FRect bckRect = {0, 0, 0, (float)(bufferHead + 1) * (unitH + 2 * dy)};
    for (int i = 0; i < NUM_HEADS; i++) bckRect.w += unitW[i] + 2 * dx;
    bckRect.x = windowRect.x + (windowRect.w - bckRect.w) / 2;
    bckRect.y = windowRect.y + (windowRect.h - bckRect.h) / 2;

    SDL_SetRenderColor(renderer, BACK_C);
    SDL_RenderFillRect(renderer, &bckRect);
    INTEL_DrawIntelSet_Head();
    INTEL_DrawIntelSet_Body();

    return true;
}

