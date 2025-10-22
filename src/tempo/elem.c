#include <math.h>
#include "elem_type/text.h"
#include "elem_type/file.h"
#include "elem_type/slid.h"
#include "elem_type/bool.h"
#include "menu.h"


// ELEM PARA ===========================================================================================================
static const SDL_FRect* publicBck = NULL;
static const Table* publicElemTable = NULL;


// ELEM TYPE ===========================================================================================================
typedef enum ElemType {
    ELEM_TYPE_NULL,
    ELEM_TYPE_FILE,
    ELEM_TYPE_TEXT,
    ELEM_TYPE_SLID,
    ELEM_TYPE_BOOL,
    ELEM_NUM_TYPES,
} ElemType;


// ELEM INFO ===========================================================================================================
typedef union ElemInfo {
    ElemFileInfo file;
    ElemTextInfo text;
    ElemSlidInfo slid;
    ElemBoolInfo bool_;
} ElemInfo;
static const struct {
    const char* name;
    bool (*create)(void*, const cJSON*);
    bool (*renew)(const void*, SDL_Texture**);
    void (*delete)(void*);
    Trig trig;
} ELEM_INFO_DETAIL[ELEM_NUM_TYPES] = {
    [ELEM_TYPE_NULL] = {"NULL", NULL, NULL, NULL, 0},
    [ELEM_TYPE_FILE] = {"FILE", TEMPO_CreateElemFile, TEMPO_RenewElemFile, TEMPO_DeleteElemFile, 0},
    [ELEM_TYPE_TEXT] = {"TEXT", TEMPO_CreateElemText, TEMPO_RenewElemText, TEMPO_DeleteElemText, 0},
    [ELEM_TYPE_SLID] = {"SLID", TEMPO_CreateElemSlid, TEMPO_RenewElemSlid, NULL, {TEMPO_TrigFuncSlid, NULL, true}},
    [ELEM_TYPE_BOOL] = {"BOOL", TEMPO_CreateElemBool, TEMPO_RenewElemBool, NULL, {TEMPO_TrigFuncBool, NULL, false}},
};
static ElemType TEMPO_GetElemTypeFromString(const char* string) {
    for (int i = 0; i < ELEM_NUM_TYPES; i++) {
        if (strcmp(string, ELEM_INFO_DETAIL[i].name) == 0) {
            return i;
        }
    }
    return ELEM_TYPE_NULL;
}


// ELEM ================================================================================================================
struct Elem {
    ElemType type;
    ElemInfo info;

    int anchor;
    SDL_Texture* tex;
    SDL_FRect gid_rect, *gid;
    SDL_FRect src_rect, *src;
    SDL_FRect dst_rect, *bck;

    Trig trig;
    char* para_string;
};


// CREATE & DELETE =====================================================================================================
static bool TEMPO_CreateElem_RK(Elem* elem, const cJSON *elem_json) {
    memset(elem, 0, sizeof(Elem));
    const char* key;
    if (cJSON_ExistKey(elem_json, key = "type")) {
        char* type_json = NULL;
        if (cJSON_Load(elem_json, key, JSM_STRING, &type_json) == true) {
            elem->type = TEMPO_GetElemTypeFromString(type_json);
        }
        REQ_CONDITION(elem->type != ELEM_TYPE_NULL, return false);
    }
    if (cJSON_ExistKey(elem_json, key = "info")) {
        const cJSON* info_json = cJSON_GetObjectItem(elem_json, key);
        REQ_CONDITION(info_json != NULL, return false);
        REQ_CONDITION(ELEM_INFO_DETAIL[elem->type].create(&elem->info, info_json), return false);
        if (ELEM_INFO_DETAIL[elem->type].trig.func != NULL) {
            elem->trig = ELEM_INFO_DETAIL[elem->type].trig;
            elem->trig.para = elem;
        }
    }
    if (cJSON_ExistKey(elem_json, key = "anchor")) {
        REQ_CONDITION(cJSON_Load(elem_json, key, JSM_INT, &elem->anchor), return false);
    }
    if (cJSON_ExistKey(elem_json, key = "gid")) {
        REQ_CONDITION(cJSON_Load(elem_json, key, JSM_FRECT, &elem->gid_rect), return false);
        elem->gid = &elem->gid_rect;
    }
    if (cJSON_ExistKey(elem_json, key = "src")) {
        REQ_CONDITION(cJSON_Load(elem_json, key, JSM_FRECT, &elem->src_rect), return false);
        elem->src = &elem->src_rect;
    }
    if (cJSON_ExistKey(elem_json, key = "func") && elem->trig.func == NULL) {
        const char* func_json = NULL;
        REQ_CONDITION(cJSON_Load(elem_json, key, JSM_STRING, &func_json), return false);

        const TrigFunc func = BASIC_GetTableValByKey(TEMPO_StaticTrigTable, func_json);
        REQ_CONDITION(func != NULL, return false);

        const char* para_json = NULL;
        if (cJSON_ExistKey(elem_json, "para")) {
            REQ_CONDITION(cJSON_Load(elem_json, "para", JSM_STRING, &para_json), return false);
        }

        if (para_json != NULL) {
            elem->para_string = strdup(para_json);
            REQ_CONDITION(elem->para_string != NULL, return false);
        }

        if (elem->trig.func ==  NULL) {
            elem->trig = (Trig){func, elem->para_string, false};
        }
    }
    if (cJSON_ExistKey(elem_json, key = "bck")) {
        const char* bck_json = NULL;
        REQ_CONDITION(cJSON_Load(elem_json, key, JSM_STRING, &bck_json), return false);
        if (publicElemTable != NULL) {
            for (int i = 0; i < publicElemTable->len; i++) {
                const char* subkey = publicElemTable->kv[i].key;
                if (subkey != NULL && strcmp(bck_json, subkey) == 0) {
                    Elem* other = publicElemTable->kv[i].val;
                    elem->bck = &other->dst_rect;
                    break;
                }
            }
        }
    }
    return true;
}
void *TEMPO_CreateElem(const cJSON *elem_json) {
    REQ_CONDITION(elem_json != NULL, return NULL);

    Elem* elem = calloc(1, sizeof(Elem));
    REQ_CONDITION(elem != NULL, return NULL);
    REQ_CONDITION(TEMPO_CreateElem_RK(elem, elem_json), elem = TEMPO_DeleteElem(elem));
    return elem;
}
void *TEMPO_DeleteElem(void *elem_void) {
    Elem* elem = elem_void;
    if (elem == NULL) return elem;

    if (ELEM_INFO_DETAIL[elem->type].delete != NULL) {
        ELEM_INFO_DETAIL[elem->type].delete(&elem->info);
    }
    if (elem->tex != NULL) {
        SDL_DestroyTexture(elem->tex);
        elem->tex = NULL;
    }
    if (elem->para_string != NULL) {
        free(elem->para_string);
        elem->para_string = NULL;
    }
    free(elem);
    elem = NULL;
    return elem;
}


// RENEW ===============================================================================================================
static bool TEMPO_RenewElem_Tex(Elem* elem) {
    if (elem->tex != NULL) {
        SDL_DestroyTexture(elem->tex);
        elem->tex = NULL;
    }
    REQ_CONDITION(ELEM_INFO_DETAIL[elem->type].renew != NULL, return false);
    REQ_CONDITION(ELEM_INFO_DETAIL[elem->type].renew(&elem->info, &elem->tex), return false);
    return true;
}
static bool TEMPO_RenewElem_DstRect(Elem *elem) {
    const bool result = SDL_LoadDstRectAligned(
        &elem->dst_rect,
        elem->tex,
        elem->src,
        elem->gid,
        elem->bck != NULL ? elem->bck : publicBck,
        elem->anchor
        );
    return result;
}
bool TEMPO_RenewElem(void *elem_void) {
    Elem* elem = elem_void;
    REQ_CONDITION(elem != NULL, return false);
    REQ_CONDITION(TEMPO_RenewElem_Tex(elem), return false);
    REQ_CONDITION(TEMPO_RenewElem_DstRect(elem), return false);

    const bool mouseIn = PERPH_GetMouseInRect(elem->dst_rect);
    const bool mouseLeftIn = PERPH_GetMouseLeftInRect(elem->dst_rect);

    if (mouseLeftIn) {
        DEBUG_SendMessageL("Elem:\n");
        DEBUG_SendMessageL("    type: %s\n", ELEM_INFO_DETAIL[elem->type].name);
        if (elem->trig.func != NULL) {
            DEBUG_SendMessageL("    trig: %s(%s)\n", BASIC_GetTableKeyByVal(TEMPO_StaticTrigTable, elem->trig.func), elem->trig.para);
        }
        DEBUG_SendMessageL("    dst: %s\n", SDL_GetStrFRect(elem->dst_rect));
    }

    if (elem->trig.func != NULL) {
        if (elem->trig.sustain && mouseLeftIn && TEMPO_OFEN_RELOAD == false) {
            PERPH_SetMouseLeftTrig(elem->trig);
        }
        if (elem->trig.sustain == false && mouseLeftIn && mouseIn && TEMPO_OFEN_RELOAD == false) {
            PERPH_SetMouseLeftTrig(elem->trig);
        }
    }

    return true;
}


// DRAW ================================================================================================================
bool TEMPO_DrawElem(const void *elem_void) {
    const Elem *elem = elem_void;
    REQ_CONDITION(elem != NULL, return false);

    const bool mouseIn = PERPH_GetMouseInRect(elem->dst_rect);
    const bool mouseLeftIn = PERPH_GetMouseLeftInRect(elem->dst_rect);
    if (mouseLeftIn) DEBUG_FillRect(elem->dst_rect);

    const SDL_FRect dst = {
        roundf(elem->dst_rect.x),
        roundf(elem->dst_rect.y),
        roundf(elem->dst_rect.w),
        roundf(elem->dst_rect.h),
    };
    const bool result = SDL_RenderTexture(renderer, elem->tex, elem->src, &dst);
    if (mouseIn || mouseLeftIn) DEBUG_DrawRect(elem->dst_rect);

    return result;
}


// TRIG ================================================================================================================
void TEMPO_TrigFuncBool(void *para) {
    const Elem* elem = para;
    bool* now = elem->info.bool_.now;
    if (now != NULL) {
        *now = !*now;
    }
}
void TEMPO_TrigFuncSlid(void *para) {
    const Elem* elem = para;
    const SDL_FRect dst_rect = elem->dst_rect;
    const ElemSlidInfo* slid = &elem->info.slid;
    TrigFunc_Slid(slid, dst_rect);
}


// SET & GET ===========================================================================================================
bool TEMPO_SetElemDstRect(Elem *elem, const SDL_FRect dst_rect) {
    if (elem == NULL) {
        printf("%s: elem is null.\n", __func__);
        return false;
    }
    elem->dst_rect = dst_rect;
    return true;
}
bool TEMPO_GetElemDstRect(const Elem *elem, SDL_FRect *dst) {
    if (elem == NULL) {
        printf("%s: elem is null.\n", __func__);
        return false;
    }
    *dst = elem->dst_rect;
    return true;
}
bool TEMPO_SetElemPublicBck(const SDL_FRect* bck) {
    if (bck == NULL) {
        return false;
    }
    publicBck = bck;
    return true;
}
bool TEMPO_SetElemPublicTable(const Table* table) {
    publicElemTable = table;
    return true;
}