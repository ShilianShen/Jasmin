#include <math.h>
#include "elem_type/text.h"
#include "menu.h"


// ELEM PARA ===========================================================================================================
static const float A = 4, B = 4, C = 6, D = 36;
static const SDL_FRect* publicBck = NULL;
static const Table* publicElemTable = NULL;


// ELEM TYPE FUNC ======================================================================================================
typedef struct ElemFileInfo {char* string;} ElemFileInfo;
typedef struct ElemTextInfo {TTF_Font* font; char* string;} ElemTextInfo;
typedef struct ElemSlidInfo {bool readonly; bool discrete; float min, max, *now;} ElemSlidInfo;
typedef struct ElemBoolInfo {bool* now;} ElemBoolInfo;
typedef union ElemInfo {
    ElemFileInfo file;
    ElemTextInfo text;
    ElemSlidInfo slid;
    ElemBoolInfo bool_;
} ElemInfo;


// ELEM TYPE ===========================================================================================================
typedef enum ElemType {
    ELEM_TYPE_NULL,
    ELEM_TYPE_FILE,
    ELEM_TYPE_TEXT,
    ELEM_TYPE_SLID,
    ELEM_TYPE_BOOL,
    ELEM_NUM_TYPES,
} ElemType;
const char* ELEM_TYPE_STRING_SET[ELEM_NUM_TYPES] = {
    [ELEM_TYPE_NULL] = "NULL",
    [ELEM_TYPE_FILE] = "FILE",
    [ELEM_TYPE_TEXT] = "TEXT",
    [ELEM_TYPE_SLID] = "SLID",
    [ELEM_TYPE_BOOL] = "BOOL",
};
static ElemType TEMPO_GetElemTypeFromString(const char* string) {
    for (int i = 0; i < ELEM_NUM_TYPES; i++) {
        if (strcmp(string, ELEM_TYPE_STRING_SET[i]) == 0) {
            return i;
        }
    }
    return ELEM_TYPE_NULL;
}



// ELEM INFO ===========================================================================================================



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

static bool TEMPO_CreateElemText(void* info, const cJSON* info_json)   {
    ElemTextInfo* text = info;
    if (cJSON_IsObject(info_json) == false) {
        return false;
    }
    const char* key = NULL;
    char* string_json = NULL;
    char* font_json = NULL;
    if (cJSON_LoadFromObj(info_json, key = "string", JSM_STRING, &string_json) == false) {
        printf("%s: failed in %s.\n", __func__, key);
        return false;
    }
    if (cJSON_LoadFromObj(info_json, key = "font", JSM_STRING, &font_json) == false) {
        printf("%s: failed in %s.\n", __func__, key);
        return false;
    }
    text->font = TABLE_GetValByKey(theme.fontTable, font_json);
    text->string = strdup(string_json);
    if (text->font == NULL || text->string == NULL) {
        return false;
    }
    return true;
}
static void TEMPO_DeleteElemText(void* info) {
    ElemTextInfo* text = info;
    if (text->string != NULL) {
        free(text->string);
        text->string = NULL;
    }
}


static bool TEMPO_CreateElemFile(void* info, const cJSON* info_json) {
    ElemFileInfo* file = info;
    const char* string_json = NULL;
    if (cJSON_IsString(info_json)) {
        string_json = info_json->valuestring;
    }
    if (string_json != NULL) {
        file->string = strdup(string_json);
        if (file->string == NULL) {
            // printf("%s: failed in %s.\n", __func__, key);
            return false;
        }
    } // Req Condition
    return true;
}
static void TEMPO_DeleteElemFile(void* info) {
    ElemFileInfo* file = info;
    if (file->string != NULL) {
        free(file->string);
        file->string = NULL;
    }
}


static bool TEMPO_CreateElemSlid(void* info, const cJSON* info_json) {
    ElemSlidInfo* slid = info;
    if (cJSON_IsObject(info_json) == false) {
        return false;
    }

    const char* key = NULL;
    const char* now_json = NULL;

    if (cJSON_LoadFromObj(info_json, key = "discrete", JSM_BOOL, &slid->discrete) == false) {
        printf("%s: failed in %s.\n", __func__, key);
        return false;
    }
    if (cJSON_LoadFromObj(info_json, key = "readonly", JSM_BOOL, &slid->readonly) == false) {
        printf("%s: failed in %s.\n", __func__, key);
        return false;
    }
    if (cJSON_LoadFromObj(info_json, key = "min", JSM_FLOAT, &slid->min) == false) {
        printf("%s: failed in %s.\n", __func__, key);
        return false;
    }
    if (cJSON_LoadFromObj(info_json, key = "max", JSM_FLOAT, &slid->max) == false) {
        printf("%s: failed in %s.\n", __func__, key);
        return false;
    }
    if (cJSON_LoadFromObj(info_json, key = "now", JSM_STRING, &now_json) == false) {
        printf("%s: failed in %s.\n", __func__, key);
        return false;
    }

    if (slid->discrete) {
        slid->min = roundf(slid->min);
        slid->max = roundf(slid->max);
    }

    const JSM_DataType type = slid->discrete ? JSM_INT : JSM_FLOAT;
    slid->now = TABLE_GetValByKey(TEMPO_ExternTable[type], now_json);
    if (slid->now == NULL) {
        printf("%s: failed in %s.\n", __func__, key);
        return false;
    }

    return true;
}
static bool TEMPO_CreateElemBool(void* info, const cJSON* info_json) {
    ElemBoolInfo* bool_ = info;
    if (cJSON_IsString(info_json) == false) {
        // printf("%s: failed in %s.\n", __func__, key);
        return false;
    }
    bool_->now = TABLE_GetValByKey(TEMPO_ExternTable[JSM_BOOL], info_json->valuestring);
    return true;
}


// ELEM EREN
typedef struct Eren {
    const char* name;
    bool (*create)(void*, const cJSON*);
    bool (*renew)(void*);
    void (*delete)(void*);
    Trig trig;
} Eren;
Eren arrElem[ELEM_NUM_TYPES] = {
    // [ELEM_TYPE_NULL] = {NULL, NULL, NULL, NULL, 0},
    [ELEM_TYPE_FILE] = {"FILE", TEMPO_CreateElemFile, NULL, TEMPO_DeleteElemFile, 0},
    [ELEM_TYPE_TEXT] = {"TEXT", TEMPO_CreateElemText, NULL, TEMPO_DeleteElemText, 0},
    [ELEM_TYPE_SLID] = {"SLID", TEMPO_CreateElemSlid, NULL, NULL, {TEMPO_TrigFuncSlid, NULL, true}},
    [ELEM_TYPE_BOOL] = {"BOOL", TEMPO_CreateElemBool, NULL, NULL, {TEMPO_TrigFuncBool, NULL, false}},
};


// CREATE & DELETE =====================================================================================================
static bool TEMPO_CreateElem_RK(Elem* elem, const cJSON *elem_json) {
    memset(elem, 0, sizeof(Elem));
    const char* key;
    if (cJSON_ExistKey(elem_json, key = "type")) {
        char* type_json = NULL;
        if (cJSON_LoadFromObj(elem_json, key, JSM_STRING, &type_json) == true) {
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
        if (cJSON_LoadFromObj(elem_json, key, JSM_INT, &elem->anchor) == false) {
            printf("%s: failed in %s.\n", __func__, key);
            return false;
        }
    }
    if (cJSON_ExistKey(elem_json, key = "gid")) {
        if (cJSON_LoadFromObj(elem_json, key, JSM_RECT, &elem->gid_rect) == false) {
            printf("%s: failed in %s.\n", __func__, key);
            return false;
        }
        elem->gid = &elem->gid_rect;
    }
    if (cJSON_ExistKey(elem_json, key = "src")) {
        if (cJSON_LoadFromObj(elem_json, key, JSM_RECT, &elem->src_rect) == false) {
            printf("%s: failed in %s.\n", __func__, key);
            return false;
        }
        elem->src = &elem->src_rect;
    }
    if (cJSON_ExistKey(elem_json, key = "func") && elem->trig.func == NULL) {
        const char* func_json = NULL;
        if (cJSON_LoadFromObj(elem_json, key, JSM_STRING, &func_json) == false) {
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
            if (cJSON_LoadFromObj(elem_json, "para", JSM_STRING, &para_json) == false) {
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
        if (cJSON_LoadFromObj(elem_json, key, JSM_STRING, &bck_json) == false) {
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
    switch (elem->type) {
        case ELEM_TYPE_FILE: {
            elem->tex = IMG_LoadTexture(renderer, elem->info.file.string);
            if (elem->tex == NULL) {
                printf("%s: failed from \"%s\".\n", __func__, elem->info.file.string);
                return NULL;
            } // Req Condition
            SDL_SetTextureScaleMode(elem->tex, SDL_SCALEMODE_NEAREST);
            break;
        }
        case ELEM_TYPE_TEXT: {
            elem->tex = TXT_LoadTextureWithLines(
                renderer,
                elem->info.text.font,
                elem->info.text.string,
                (SDL_Color){255, 255, 255, 255},
                EMPTY,
                'C'
                );
            if (elem->tex == NULL) {
                printf("%s: failed from \"%s\".\n", __func__, elem->info.file.string);
                return NULL;
            } // Req Condition
            SDL_SetTextureScaleMode(elem->tex, SDL_SCALEMODE_NEAREST);
            break;
        }
        case ELEM_TYPE_SLID: {
            const float M = elem->info.slid.max - elem->info.slid.min;
            const float W = 2 * A + (M + 1) * B + M * C;
            const float H = 2 * A + 2 * B + D;
            elem->src_rect = (SDL_FRect){0, 0, W, H};
            elem->gid_rect.w = elem->gid_rect.h = 1;
            elem->tex = SDL_CreateTexture(
            renderer,
            SDL_PIXELFORMAT_RGBA8888,
            SDL_TEXTUREACCESS_TARGET,
            (int)W, (int)H
            );
            SDL_SetRenderTarget(renderer, elem->tex);
            SDL_SetRenderDrawColor(renderer, 255, 255, 200, 255);
            const SDL_FRect frame[4] = {
                (SDL_FRect){0, 0, W, A},
                (SDL_FRect){0, 0, A, H},
                (SDL_FRect){0, H - A, W, A},
                (SDL_FRect){W - A, 0, A, H},
            };
            SDL_RenderFillRects(renderer, frame, 4);
            if (elem->info.slid.discrete) {
                if (elem->info.slid.now != NULL) {
                    const int N = *(int*)elem->info.slid.now - (int)elem->info.slid.min;
                    SDL_FRect rects[N];
                    for (int i = 0; i < N; i++) {
                        rects[i].x = A + (float)(i + 1) * B + (float)i * C;
                        rects[i].y = A + B;
                        rects[i].w = C;
                        rects[i].h = D;
                    }
                    SDL_RenderFillRects(renderer, rects, N);
                }
                else {
                    SDL_RenderLine(renderer, 0, 0, W, H);
                    SDL_RenderLine(renderer, W, 0, 0, H);
                }
            }
            else {
                if (elem->info.slid.now != NULL) {
                    const float N = *elem->info.slid.now - elem->info.slid.min;
                    const SDL_FRect rect = {A + B, A + B, (W - 2 * A - 2 * B) * N / M, H - 2 * A - 2 * B};
                    SDL_RenderFillRect(renderer, &rect);
                }
                else {
                    SDL_RenderLine(renderer, 0, 0, W, H);
                    SDL_RenderLine(renderer, W, 0, 0, H);
                }
            }
            SDL_SetRenderTarget(renderer, NULL);
            break;
        }
        case ELEM_TYPE_BOOL: {
            const float M = 1;
            const float N = *elem->info.bool_.now;
            const float W = 2 * A + (M + 1) * B + M * D;
            const float H = 2 * A + 2 * B + D;
            elem->src_rect = (SDL_FRect){0, 0, W, H};
            elem->gid_rect.w = elem->gid_rect.h = 1;
            elem->tex = SDL_CreateTexture(
               renderer,
               SDL_PIXELFORMAT_RGBA8888,
               SDL_TEXTUREACCESS_TARGET,
               (int)W, (int)H
               );
            SDL_SetRenderTarget(renderer, elem->tex);
            SDL_SetRenderDrawColor(renderer, 255, 255, 200, 255);
            const SDL_FRect frame[5] = {
                (SDL_FRect){0, 0, W, A},
                (SDL_FRect){0, 0, A, H},
                (SDL_FRect){0, H - A, W, A},
                (SDL_FRect){W - A, 0, A, H},
                (SDL_FRect){A + B, A + B, (W - 2 * A - 2 * B) * N / M, H - 2 * A - 2 * B},
            };
            SDL_RenderFillRects(renderer, frame, 5);
            SDL_SetRenderTarget(renderer, NULL);
            break;
        }
        default: break;
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

    const bool mouseIn = DEVICE_MouseInRect(&elem->dst_rect);
    const bool mouseLeftIn = DEVICE_MouseLeftInRect(&elem->dst_rect);

    if (mouseLeftIn) {
        DEBUG_SendMessageL("Elem:\n");
        DEBUG_SendMessageL("    type: %s\n", ELEM_TYPE_STRING_SET[elem->type]);
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
    const bool mouseIn = DEVICE_MouseInRect(&elem->dst_rect);
    const bool mouseLeftIn = DEVICE_MouseLeftInRect(&elem->dst_rect);
    if (mouseLeftIn) {
        DEBUG_FillRect(&elem->dst_rect);
    }
    const SDL_FRect dst = {
        roundf(elem->dst_rect.x),
        roundf(elem->dst_rect.y),
        roundf(elem->dst_rect.w),
        roundf(elem->dst_rect.h),
    };
    SDL_RenderTexture(renderer, elem->tex, elem->src, &dst);
    if (mouseIn || mouseLeftIn) {
        DEBUG_DrawRect(&elem->dst_rect);
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
    if (elem->info.slid.readonly) {
        return;
    }
    const float min = elem->dst_rect.x + A + B;
    const float max = elem->dst_rect.x + elem->dst_rect.w - A - B;
    const float now = DEVICE_GetMousePos().x;
    if (elem->info.slid.discrete) {
        if (now <= min)
            *(int*)elem->info.slid.now = (int)elem->info.slid.min;
        else if (now >= max)
            *(int*)elem->info.slid.now = (int)elem->info.slid.max;
        else
            *(int*)elem->info.slid.now = (int)((now - min) / (B + C) + elem->info.slid.min);
    }
    else {
        if (now <= min)
            *elem->info.slid.now = elem->info.slid.min;
        else if (now >= max)
            *elem->info.slid.now = elem->info.slid.max;
        else
            *elem->info.slid.now = elem->info.slid.min + (elem->info.slid.max - elem->info.slid.min) * (now - min) / (max - min);
    }
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