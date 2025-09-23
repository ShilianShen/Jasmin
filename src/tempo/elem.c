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
struct {
    const char* name;
    bool (*create)(void*, const cJSON*);
    bool (*renew)(const void*, SDL_Texture**);
    void (*delete)(void*);
    Trig trig;
} arrElem[ELEM_NUM_TYPES] = {
    [ELEM_TYPE_NULL] = {"NULL", NULL, NULL, NULL, 0},
    [ELEM_TYPE_FILE] = {"FILE", TEMPO_CreateElemFile, TEMPO_RenewElemFile, TEMPO_DeleteElemFile, 0},
    [ELEM_TYPE_TEXT] = {"TEXT", TEMPO_CreateElemText, TEMPO_RenewElemText, TEMPO_DeleteElemText, 0},
    [ELEM_TYPE_SLID] = {"SLID", TEMPO_CreateElemSlid, TEMPO_RenewElemSlid, NULL, {TEMPO_TrigFuncSlid, NULL, true}},
    [ELEM_TYPE_BOOL] = {"BOOL", TEMPO_CreateElemBool, TEMPO_RenewElemBool, NULL, {TEMPO_TrigFuncBool, NULL, false}},
};
static ElemType TEMPO_GetElemTypeFromString(const char* string) {
    for (int i = 0; i < ELEM_NUM_TYPES; i++) {
        if (strcmp(string, arrElem[i].name) == 0) {
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
        if (elem->type == ELEM_TYPE_NULL) {
            printf("%s: failed in %s.\n", __func__, key);
            return false;
        } // Req Condition
    }
    if (cJSON_ExistKey(elem_json, key = "info")) {
        const cJSON* info_json = cJSON_GetObjectItem(elem_json, key);
        if (info_json == NULL) {
            printf("%s: failed in %s.\n", __func__, key);
            return false;
        } // Req Condition
        if (arrElem[elem->type].create(&elem->info, info_json) == false) {
            printf("%s: failed in %s.\n", __func__, key);
            return false;
        }
        if (arrElem[elem->type].trig.func != NULL) {
            elem->trig = arrElem[elem->type].trig;
            elem->trig.para = elem;
        }
    }
    if (cJSON_ExistKey(elem_json, key = "anchor")) {
        if (cJSON_Load(elem_json, key, JSM_INT, &elem->anchor) == false) {
            printf("%s: failed in %s.\n", __func__, key);
            return false;
        }
    }
    if (cJSON_ExistKey(elem_json, key = "gid")) {
        if (cJSON_Load(elem_json, key, JSM_FRECT, &elem->gid_rect) == false) {
            printf("%s: failed in %s.\n", __func__, key);
            return false;
        }
        elem->gid = &elem->gid_rect;
    }
    if (cJSON_ExistKey(elem_json, key = "src")) {
        if (cJSON_Load(elem_json, key, JSM_FRECT, &elem->src_rect) == false) {
            printf("%s: failed in %s.\n", __func__, key);
            return false;
        }
        elem->src = &elem->src_rect;
    }
    if (cJSON_ExistKey(elem_json, key = "func") && elem->trig.func == NULL) {
        const char* func_json = NULL;
        if (cJSON_Load(elem_json, key, JSM_STRING, &func_json) == false) {
            printf("%s: failed in %s.\n", __func__, key);
            return false;
        }

        const TrigFunc func = TABLE_GetValByKey(TEMPO_StaticTrigTable, func_json);
        if (func == NULL) {
            printf("%s: failed in %s.\n", __func__, key);
            return false;
        }

        const char* para_json = NULL;
        if (cJSON_ExistKey(elem_json, "para")) {
            if (cJSON_Load(elem_json, "para", JSM_STRING, &para_json) == false) {
                printf("%s: para == NULL, %s.\n", __func__, key);
                return false;
            }
        }

        if (para_json != NULL) {
            elem->para_string = strdup(para_json);
            if (elem->para_string == NULL) {
                printf("%s: failed in %s.\n", __func__, key);
                return false;
            }
        }

        if (elem->trig.func ==  NULL) {
            elem->trig = (Trig){func, elem->para_string, false};
        }
    }
    if (cJSON_ExistKey(elem_json, key = "bck")) {
        const char* bck_json = NULL;
        if (cJSON_Load(elem_json, key, JSM_STRING, &bck_json) == false) {
            printf("%s: failed in %s.\n", __func__, key);
            return false;
        }
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
Elem* TEMPO_DeleteElem(Elem *elem) {
    if (elem == NULL) {
        return elem;
    } // Opt Condition

    if (arrElem[elem->type].delete != NULL) {
        arrElem[elem->type].delete(&elem->info);
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
Elem* TEMPO_CreateElem(const cJSON *elem_json) {
    if (elem_json == NULL) {
        printf("%s: tomlElem == NULL\n", __func__);
        return NULL;
    } // Req Condition
    Elem* elem = calloc(1, sizeof(Elem));
    if (elem == NULL) {
        printf("%s: elem == NULL.\n", __func__);
        return elem;
    } // Req Condition
    if (TEMPO_CreateElem_RK(elem, elem_json) == false) {
        printf("%s: RK == false.\n", __func__);
        elem = TEMPO_DeleteElem(elem);
    } // Req Condition
    return elem;
}


// RENEW ===============================================================================================================
static bool TEMPO_RenewElem_Tex(Elem* elem) {
    if (elem->tex != NULL) {
        SDL_DestroyTexture(elem->tex);
        elem->tex = NULL;
    }

    if (arrElem[elem->type].renew == NULL || arrElem[elem->type].renew(&elem->info, &elem->tex) == false) {
        printf("%s: failed in %s.\n", __func__, "asd");
        return false;
    }

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
bool TEMPO_RenewElem(Elem *elem) {
    if (TEMPO_RenewElem_Tex(elem) == false) {
        printf("%s: TEMPO_RenewElemTex(elem) == false\n", __func__);
        return false;
    }
    if (TEMPO_RenewElem_DstRect(elem) == false) {
        printf("%s: TEMPO_RenewElemDstRect(elem) == false\n", __func__);
        return false;
    }

    const bool mouseIn = DEVICE_GetMouseInRect(elem->dst_rect);
    const bool mouseLeftIn = DEVICE_GetMouseLeftInRect(elem->dst_rect);

    if (mouseLeftIn) {
        DEBUG_SendMessageL("Elem:\n");
        DEBUG_SendMessageL("    type: %s\n", arrElem[elem->type].name);
        // DEBUG_SendMessageL("    info: %s\n", elem->info);
        if (elem->trig.func != NULL) {
            DEBUG_SendMessageL("    trig: %s(%s)\n", TABLE_GetKeyByVal(TEMPO_StaticTrigTable, elem->trig.func), elem->trig.para);
        }
        DEBUG_SendMessageL("    dst: %s\n", SDL_GetStringFromFRect(elem->dst_rect));
    }

    if (elem->trig.func != NULL) {
        if (elem->trig.sustain && mouseLeftIn && TEMPO_OFEN_RELOAD == false) {
            DEVICE_SetMouseLeftTrig(&elem->trig);
        }
        if (elem->trig.sustain == false && mouseLeftIn && mouseIn && TEMPO_OFEN_RELOAD == false) {
            DEVICE_SetMouseLeftTrig(&elem->trig);
        }
    }

    return true;
}


// DRAW ================================================================================================================
bool TEMPO_DrawElem(const Elem *elem) {
    // Req Condition
    if (renderer == NULL) {
        DEBUG_SendMessageR("%s: menu.renderer is NULL.\n", __func__);
        return false;
    }
    const bool mouseIn = DEVICE_GetMouseInRect(elem->dst_rect);
    const bool mouseLeftIn = DEVICE_GetMouseLeftInRect(elem->dst_rect);
    if (mouseLeftIn) {
        DEBUG_FillRect(elem->dst_rect);
    }
    const SDL_FRect dst = {
        roundf(elem->dst_rect.x),
        roundf(elem->dst_rect.y),
        roundf(elem->dst_rect.w),
        roundf(elem->dst_rect.h),
    };
    SDL_RenderTexture(renderer, elem->tex, elem->src, &dst);
    if (mouseIn || mouseLeftIn) {
        DEBUG_DrawRect(elem->dst_rect);
    }
    return true;
}


// TRIG ================================================================================================================
void TEMPO_TrigFuncBool(const void *para) {
    const Elem* elem = para;
    bool* now = elem->info.bool_.now;
    if (now != NULL) {
        *now = !*now;
    }
}
void TEMPO_TrigFuncSlid(const void *para) {
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