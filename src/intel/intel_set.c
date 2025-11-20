#include "intel_set.h"
#include "entity.h"
#include "action.h"
#include "intel_arr.h"


typedef enum SetType {TYPE_VISIBLE, TYPE_ENTITY, TYPE_ACTION, TYPE_JUDGE, TYPE_STATE, NUM_TYPES} SetType;
typedef struct TypeInfo {int num; const char** names; TrigFunc func; SDL_Texture** tex; float w;} TypeInfo;
TypeInfo typeInfos[NUM_TYPES];


typedef enum SetHead {HEAD_VISIBLE, HEAD_SUBJECT, HEAD_ACTION, HEAD_OBJECT, HEAD_JUDGE, HEAD_STATE, NUM_HEADS} SetHead;
typedef struct HeadInfo{
    const char* name; SetType type; TrigFunc func; SDL_Texture* tex; float w; SDL_FRect rect;
} HeadInfo;
HeadInfo headInfos[NUM_HEADS] = {
    [HEAD_VISIBLE] = {"VISIBLE", TYPE_VISIBLE, INTEL_TrigSortVisible},
    [HEAD_SUBJECT] = {"SUBJECT", TYPE_ENTITY, INTEL_TrigSortSubject},
    [HEAD_ACTION] = {"ACTION", TYPE_ACTION, INTEL_TrigSortAction},
    [HEAD_OBJECT] = {"OBJECT", TYPE_ENTITY, INTEL_TrigSortObject},
    [HEAD_JUDGE] = {"JUDGE", TYPE_JUDGE, INTEL_TrigSortJudge},
    [HEAD_STATE] = {"STATE", TYPE_STATE, INTEL_TrigSortState},
};
static float unitH = 0;
static const float dx = 10, dy = 5;
static const SDL_Color BACK_C = {64, 64, 64, 128};


static const int LEN_BUFFER = 100;
static struct {Intel* intel; SDL_FRect rect[NUM_HEADS];} buffer[LEN_BUFFER];
static int bufferHead = 0;


// GET & SET ===========================================================================================================
SDL_FRect INTEL_GetIntelSetBckRect() {
    SDL_FRect bckRect = {0, 0, 0, (float)(bufferHead + 1) * (unitH + 2 * dy)};
    for (int i = 0; i < NUM_HEADS; i++) bckRect.w += headInfos[i].w + 2 * dx;
    bckRect.x = windowRect.x + (windowRect.w - bckRect.w) / 2;
    bckRect.y = windowRect.y + (windowRect.h - bckRect.h) / 2;
    return bckRect;
}


// TRIG ================================================================================================================
void INTEL_TrigChangeVisible(TrigPara para) {
    Intel* intel = (Intel*)para;
    intel->visible = !intel->visible;
}
void INTEL_TrigChangeJudge(TrigPara para) {
    Intel* intel = (Intel*)para;
    intel->judge = (intel->judge + 1) % NUM_JUDGES;
    intel->state = STATE_UNKNOWN;
}
void INTEL_TrigChangeState(TrigPara para) {
    Intel* intel = (Intel*)para;
    if (intel->judge != JUDGE_MANU) return;
    intel->state = (intel->state + 1) % NUM_STATES;
}


void INTEL_TrigSortVisible(TrigPara para) {
    const IntelArr* intelArr = (IntelArr*)para;
    for (int i = 0; i < intelArr->len; i++) {
        if (intelArr->arr[i].visible == true) continue;

        for (int j = i + 1; j < intelArr->len; j++) {
            if (intelArr->arr[j].visible == false) continue;

            const Intel temp = intelArr->arr[i];
            intelArr->arr[i] = intelArr->arr[j];
            intelArr->arr[j] = temp;
            break;
        }
    }
}
void INTEL_TrigSortJudge(TrigPara para) {
    const IntelArr* intelArr = (IntelArr*)para;
    for (int i = 0; i < intelArr->len; i++) {
        for (int j = i + 1; j < intelArr->len; j++) {
            if (intelArr->arr[i].judge <= intelArr->arr[j].judge) continue;
            const Intel temp = intelArr->arr[i];
            intelArr->arr[i] = intelArr->arr[j];
            intelArr->arr[j] = temp;
        }
    }
}
void INTEL_TrigSortSubject(TrigPara para) {
    const IntelArr* intelArr = (IntelArr*)para;
    for (int i = 0; i < intelArr->len; i++) {
        for (int j = i + 1; j < intelArr->len; j++) {
            if (intelArr->arr[i].subject <= intelArr->arr[j].subject) continue;
            const Intel temp = intelArr->arr[i];
            intelArr->arr[i] = intelArr->arr[j];
            intelArr->arr[j] = temp;
        }
    }
}
void INTEL_TrigSortAction(TrigPara para) {
    const IntelArr* intelArr = (IntelArr*)para;
    for (int i = 0; i < intelArr->len; i++) {
        for (int j = i + 1; j < intelArr->len; j++) {
            if (intelArr->arr[i].action <= intelArr->arr[j].action) continue;
            const Intel temp = intelArr->arr[i];
            intelArr->arr[i] = intelArr->arr[j];
            intelArr->arr[j] = temp;
        }
    }
}
void INTEL_TrigSortObject(TrigPara para) {
    const IntelArr* intelArr = (IntelArr*)para;
    for (int i = 0; i < intelArr->len; i++) {
        for (int j = i + 1; j < intelArr->len; j++) {
            if (intelArr->arr[i].object <= intelArr->arr[j].object) continue;
            const Intel temp = intelArr->arr[i];
            intelArr->arr[i] = intelArr->arr[j];
            intelArr->arr[j] = temp;
        }
    }
}
void INTEL_TrigSortState(TrigPara para) {
    const IntelArr* intelArr = (IntelArr*)para;
    for (int i = 0; i < intelArr->len; i++) {
        for (int j = i + 1; j < intelArr->len; j++) {
            if (intelArr->arr[i].state <= intelArr->arr[j].state) continue;
            const Intel temp = intelArr->arr[i];
            intelArr->arr[i] = intelArr->arr[j];
            intelArr->arr[j] = temp;
        }
    }
}


// INIT & EXIT =========================================================================================================
static bool INTEL_InitIntelSet_Type() {
    static const char* VISIBLE_NAMES[2] = {"FALSE", "TRUE"};
    typeInfos[TYPE_VISIBLE] = (TypeInfo){2, VISIBLE_NAMES, INTEL_TrigChangeVisible};
    typeInfos[TYPE_ENTITY] = (TypeInfo){NUM_ENTITIES, ENTITY_NAMES};
    typeInfos[TYPE_ACTION] = (TypeInfo){NUM_ACTIONS, ACTION_NAMES};
    typeInfos[TYPE_JUDGE] = (TypeInfo){NUM_JUDGES, JUDGE_NAMES, INTEL_TrigChangeJudge};
    typeInfos[TYPE_STATE] = (TypeInfo){NUM_STATES, STATE_NAMES, INTEL_TrigChangeState};
    for (int i = 0; i < NUM_TYPES; i++) {
        typeInfos[i].tex = calloc(typeInfos[i].num, sizeof(SDL_Texture*));
        REQ_CONDITION(typeInfos[i].tex != NULL, return false);
    }
    return true;
}
static bool INTEL_InitIntelSet_RK(TTF_Font* font) {
    REQ_CONDITION(font != NULL, return false);

    for (int I = 0; I < NUM_TYPES; I++) {
        const int num = typeInfos[I].num;
        const char** names = typeInfos[I].names;
        SDL_Texture** tex = typeInfos[I].tex;

        for (int i = 0; i < num; i++) {
            tex[i] = TXT_LoadTexture(renderer, font, names[i], WHITE);
            REQ_CONDITION(tex[i] != NULL, return false);
            typeInfos[I].w = SDL_max(typeInfos[I].w, tex[i]->w);
        }
    }

    for (int i = 0; i < NUM_HEADS; i++) {
        headInfos[i].tex = TXT_LoadTexture(renderer, font, headInfos[i].name, WHITE);
        REQ_CONDITION(headInfos[i].tex != NULL, return false);
        headInfos[i].w = SDL_max(typeInfos[headInfos[i].type].w, headInfos[i].tex->w);
    }
    unitH = (float)TTF_GetFontHeight(font);
    return true;
}
bool INTEL_InitIntelSet() {
    REQ_CONDITION(INTEL_InitIntelSet_Type(), return false);

    const bool rk = INTEL_InitIntelSet_RK(BASIC_GetFont("intel_set"));

    REQ_CONDITION(rk != false, return false);
    return true;
}
void INTEL_ExitIntelSet() {
    for (int I = 0; I < NUM_TYPES; I++) {
        const int num = typeInfos[I].num;
        SDL_Texture** tex = typeInfos[I].tex;
        for (int i = 0; i < num; i++) {
            SDL_DestroyTexture(tex[i]);
            tex[i] = NULL;
        }
        free(typeInfos[I].tex);
        typeInfos[I].tex = NULL;
    }

    for (int i = 0; i < NUM_HEADS; i++) {
        SDL_DestroyTexture(headInfos[i].tex);
        headInfos[i].tex = NULL;
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
        SDL_FRect rect = {.w = (float)headInfos[i].tex->w, .h = (float)headInfos[i].tex->h};
        rect.x = x + (headInfos[i].w - rect.w) / 2;
        rect.y = y + (unitH - rect.h) / 2;
        headInfos[i].rect = rect;
        x += headInfos[i].w + dx;
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
            int index;
            switch (j) {
                case HEAD_VISIBLE: index = intel.visible; break;
                case HEAD_SUBJECT: index = intel.subject; break;
                case HEAD_ACTION: index = intel.action; break;
                case HEAD_OBJECT: index = intel.object; break;
                case HEAD_JUDGE: index = intel.judge; break;
                case HEAD_STATE: index = intel.state; break;
                default: continue;
            }
            const SDL_Texture* tx = typeInfos[headInfos[j].type].tex[index];
            SDL_FRect rect = {.w = (float)tx->w, .h = (float)tx->h};
            rect.x = x + (headInfos[j].w - rect.w) / 2;
            rect.y = y + (unitH - rect.h) / 2;
            buffer[i].rect[j] = rect;
            x += headInfos[j].w + dx;
        }
    }
    return true;
}
static bool INTEL_RenewIntelSet_Trig(IntelArr* intelArr) {
    for (int i = 0; i < NUM_HEADS; i++) {
        const SDL_FRect rect = headInfos[i].rect;
        if (PERPH_GetMouseAndKeyInRect(PERPH_MOUSE_KEY_LEFT, rect) == false) continue;
        PERPH_SetMouseKeyTrig(PERPH_MOUSE_KEY_LEFT, (Trig){headInfos[i].func, (TrigPara)intelArr, false});
    }
    for (int i = 0; i < bufferHead; i++) {
        Intel* intel = buffer[i].intel;
        for (int j = 0; j < NUM_HEADS; j++) {
            const SDL_FRect rect = buffer[i].rect[j];
            if (PERPH_GetMouseAndKeyInRect(PERPH_MOUSE_KEY_LEFT, rect) == false) continue;
            const SetType type = headInfos[j].type;
            PERPH_SetMouseKeyTrig(PERPH_MOUSE_KEY_LEFT, (Trig){typeInfos[type].func, (TrigPara)intel, false});
        }
    }
    return true;
}
bool INTEL_RenewIntelSet(IntelArr* intelArr) {
    REQ_CONDITION(INTEL_RenewIntelSet_Buffer(intelArr), return false);

    const SDL_FRect bckRect = INTEL_GetIntelSetBckRect();

    REQ_CONDITION(INTEL_RenewIntelSet_Head(bckRect), return false);
    REQ_CONDITION(INTEL_RenewIntelSet_Body(bckRect), return false);
    REQ_CONDITION(INTEL_RenewIntelSet_Trig(intelArr), return false);
    return true;
}


// DRAW ================================================================================================================
static bool INTEL_DrawIntelSet_Head() {
    for (int i = 0; i < NUM_HEADS; i++) {
        SDL_FRect rect = headInfos[i].rect;
        SDL_RenderTexture(renderer, headInfos[i].tex, NULL, &rect);
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
                case HEAD_VISIBLE: tx = typeInfos[TYPE_VISIBLE].tex[intel.visible]; break;
                case HEAD_SUBJECT: tx = typeInfos[TYPE_ENTITY].tex[intel.subject]; break;
                case HEAD_ACTION: tx = typeInfos[TYPE_ACTION].tex[intel.action]; break;
                case HEAD_OBJECT: tx = typeInfos[TYPE_ENTITY].tex[intel.object]; break;
                case HEAD_JUDGE: {
                    back = BACK_C;
                    tx = typeInfos[TYPE_JUDGE].tex[intel.judge];
                    break;
                }
                case HEAD_STATE: {
                    tx = typeInfos[TYPE_STATE].tex[intel.state];
                    back = STATE_COLORS[intel.state];
                    text = JUDGE_COLORS[intel.judge];
                    break;
                }
                default: continue;
            }
            const SDL_FRect rect = buffer[i].rect[j];
            if (PERPH_GetMouseAndKeyInRect(PERPH_MOUSE_KEY_LEFT, rect)) {
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
    for (int i = 0; i < NUM_HEADS; i++) bckRect.w += headInfos[i].w + 2 * dx;
    bckRect.x = windowRect.x + (windowRect.w - bckRect.w) / 2;
    bckRect.y = windowRect.y + (windowRect.h - bckRect.h) / 2;

    SDL_SetRenderColor(renderer, BACK_C);
    SDL_RenderFillRect(renderer, &bckRect);
    INTEL_DrawIntelSet_Head();
    INTEL_DrawIntelSet_Body();

    return true;
}

