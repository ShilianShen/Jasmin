#include <math.h>

#include "menu.h"


// ELEM PARA ===========================================================================================================
static const float A = 4, B = 4, C = 6, D = 36;
static const SDL_FRect* publicBck = NULL;


// static const KeyVal* publicElemTable = NULL;
// static int lenPublicElemTable = 0;
static const Table* publicTable = NULL;


// ELEM TYPE ===========================================================================================================
typedef enum ElemType {
    ELEM_TYPE_NULL,
    ELEM_TYPE_FILE,
    ELEM_TYPE_TEXT,
    ELEM_TYPE_HEAD_1,
    ELEM_TYPE_HEAD_2,
    ELEM_TYPE_HEAD_3,
    ELEM_TYPE_HEAD_4,
    ELEM_TYPE_SLID_F,
    ELEM_TYPE_SLID_I,
    ELEM_TYPE_SWITCH,
    ELEM_NUM_TYPES,
} ElemType;
const char* ELEM_TYPE_STRING_SET[ELEM_NUM_TYPES] = {
    [ELEM_TYPE_NULL] = "NULL",
    [ELEM_TYPE_FILE] = "FILE",

    [ELEM_TYPE_TEXT] = "TEXT",
    [ELEM_TYPE_HEAD_1] = "HEAD_1",
    [ELEM_TYPE_HEAD_2] = "HEAD_2",
    [ELEM_TYPE_HEAD_3] = "HEAD_3",
    [ELEM_TYPE_HEAD_4] = "HEAD_4",

    [ELEM_TYPE_SLID_F] = "SLID_F",
    [ELEM_TYPE_SLID_I] = "SLID_I",
    [ELEM_TYPE_SWITCH] = "SWITCH",
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
typedef union ElemInfo {
    char* string;
    struct SlidI {int min, max, *now;} slidI;
    struct SlidF {float min, max, *now;} slidF;
    struct Switch {bool* now;} switch_;
} ElemInfo;


// ELEM ================================================================================================================
struct Elem {
    ElemType type;
    ElemInfo info;

    int anchor;
    SDL_Texture* tex;
    SDL_FRect gid_rect, *gid;
    SDL_FRect src_rect, *src;
    SDL_FRect dst_rect, *bck;

    Trig* trig;
};


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
    publicTable = table;
    return true;
}


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
        switch (elem->type) {
            case ELEM_TYPE_TEXT:
            case ELEM_TYPE_FILE: {
                char* string_json;
                if (cJSON_LoadFromObj(elem_json, "info", JSM_STRING, &string_json) == false) {
                    printf("%s: failed in %s.\n", __func__, key);
                    return false;
                } // Req Condition
                if (string_json != NULL) {
                    elem->info.string = strdup(string_json);
                    if (elem->info.string == NULL) {
                        printf("%s: failed in %s.\n", __func__, key);
                        return false;
                    }
                } // Req Condition
                break;
            }
            case ELEM_TYPE_SLID_I: {
                if (cJSON_IsObject(info_json) == false) {
                    printf("%s: failed in %s.\n", __func__, key);
                    return false;
                }
                const bool min = cJSON_LoadFromObj(info_json, "min", JSM_INT, &elem->info.slidI.min);
                const bool max = cJSON_LoadFromObj(info_json, "max", JSM_INT, &elem->info.slidI.max);
                const Table table = TEMPO_ExternTable[JSM_INT];
                const bool now = cJSON_LoadFromTab(info_json, "now", (void**)&elem->info.slidI.now, table.len, table.kv);
                if ((min && max && now) == false) {
                    printf("%s: failed in %s.\n", __func__, key);
                    return false;
                }
                break;
            }
            case ELEM_TYPE_SLID_F: {
                if (cJSON_IsObject(info_json) == false) {
                    printf("%s: failed in %s.\n", __func__, key);
                    return false;
                }
                const bool min = cJSON_LoadFromObj(info_json, "min", JSM_FLOAT, &elem->info.slidF.min);
                const bool max = cJSON_LoadFromObj(info_json, "max", JSM_FLOAT, &elem->info.slidF.max);
                const Table table = TEMPO_ExternTable[JSM_FLOAT];
                const bool now = cJSON_LoadFromTab(info_json, "now", (void**)&elem->info.slidF.now, table.len, table.kv);
                if ((min && max && now) == false) {
                    printf("%s: failed in %s.\n", __func__, key);
                    return false;
                }
                break;
            }
            case ELEM_TYPE_SWITCH: {
                if (cJSON_IsString(info_json) == false) {
                    printf("%s: failed in %s.\n", __func__, key);
                    return false;
                }
                // elem->info.switch_.now = BASIC_GetValByKey(TEMPO_PUBLIC_BOOL_LEN, TEMPO_PUBLIC_BOOL, info_json->valuestring);
                elem->info.switch_.now = TABLE_GetValByKey(TEMPO_ExternTable[JSM_BOOL], info_json->valuestring);
                elem->trig = CreateTrig(TEMPO_TrigFuncSwitch, info_json->valuestring);
                if (elem->trig == NULL) {
                    printf("%s: failed in %s.\n", __func__, key);
                    return false;
                }
                break;
            }
            default: break;
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
    if (cJSON_ExistKey(elem_json, key = "func")) {
        const char* funcName = NULL;
        if (cJSON_LoadFromObj(elem_json, key, JSM_STRING, &funcName) == false) {
            printf("%s: failed in %s.\n", __func__, key);
            return false;
        }
        const TrigFunc func = TABLE_GetValByKey(TEMPO_StaticTrigTable, funcName);
        if (func == NULL) {
            printf("%s: failed in %s.\n", __func__, key);
            return false;
        }
        const char* para = NULL;
        if (cJSON_ExistKey(elem_json, "para")) {
            if (cJSON_LoadFromObj(elem_json, "para", JSM_STRING, &para) == false) {
                printf("%s: para == NULL, %s.\n", __func__, key);
                return false;
            }
        }
        if (elem->trig == NULL) {
            elem->trig = CreateTrig(func, para);
            if (elem->trig == NULL) {
                printf("%s: failed in %s.\n", __func__, key);
                return false;
            }
        }
    }
    if (cJSON_ExistKey(elem_json, key = "bck")) {
        const char* bck_json = NULL;
        if (cJSON_LoadFromObj(elem_json, key, JSM_STRING, &bck_json) == false) {
            printf("%s: failed in %s.\n", __func__, key);
            return false;
        }
        if (publicTable != NULL) {
            for (int i = 0; i < publicTable->len; i++) {
                const char* subkey = publicTable->kv[i].key;
                if (subkey != NULL && strcmp(bck_json, subkey) == 0) {
                    Elem* other = publicTable->kv[i].val;
                    elem->bck = &other->dst_rect;
                    break;
                }
            }
        }
    }
    return true;
}
static bool TEMPO_CreateElem_CK(const Elem* elem) {
    if ((elem->type == ELEM_TYPE_FILE || elem->type == ELEM_TYPE_TEXT) && elem->info.string == NULL) {
        printf("%s: info == NULL.\n", __func__);
        return false;
    } // Req Condition
    return true;
}
Elem* TEMPO_DeleteElem(Elem *elem) {
    if (elem == NULL) {
        return elem;
    } // Opt Condition
    switch (elem->type) {
        case ELEM_TYPE_TEXT: {}
        case ELEM_TYPE_FILE: {
            if (elem->info.string != NULL) {
                free(elem->info.string);
                elem->info.string = NULL;
            }
            break;
        }
        default: break;
    }
    if (elem->trig != NULL) {
        elem->trig = DeleteTrig(elem->trig);
    }
    if (elem->tex != NULL) {
        SDL_DestroyTexture(elem->tex);
        elem->tex = NULL;
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
    if (TEMPO_CreateElem_RK(elem, elem_json) == false || TEMPO_CreateElem_CK(elem) == false) {
        printf("%s: RK or CK == false.\n", __func__);
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
            elem->tex = IMG_LoadTexture(renderer, elem->info.string);
            if (elem->tex == NULL) {
                printf("%s: failed from \"%s\".\n", __func__, elem->info.string);
                return NULL;
            } // Req Condition
            SDL_SetTextureScaleMode(elem->tex, SDL_SCALEMODE_NEAREST);
            break;
        }
        case ELEM_TYPE_TEXT: {
            elem->tex = TXT_LoadTextureWithLines(
                renderer,
                theme.textFont,
                elem->info.string,
                (SDL_Color){255, 255, 255, 255},
                EMPTY,
                'C'
                );
            if (elem->tex == NULL) {
                printf("%s: failed from \"%s\".\n", __func__, elem->info.string);
                return NULL;
            } // Req Condition
            SDL_SetTextureScaleMode(elem->tex, SDL_SCALEMODE_NEAREST);
            break;
        }
        case ELEM_TYPE_SLID_I:
        case ELEM_TYPE_SLID_F: {
            const float M = elem->type == ELEM_TYPE_SLID_I
                    ? (float)(elem->info.slidI.max - elem->info.slidI.min)
                    : elem->info.slidF.max - elem->info.slidF.min;
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
            if (elem->type == ELEM_TYPE_SLID_I) {
                if (elem->info.slidI.now != NULL) {
                    const int N = *elem->info.slidI.now - elem->info.slidI.min;
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
            if (elem->type == ELEM_TYPE_SLID_F) {
                if (elem->info.slidF.now != NULL) {
                    const float N = *elem->info.slidF.now - elem->info.slidF.min;
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
        case ELEM_TYPE_SWITCH: {
            const float M = 1;
            const float N = *elem->info.switch_.now;
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
        if (elem->trig != NULL) {
            DEBUG_SendMessageL("    trig: %s(%s)\n", TABLE_GetKeyByVal(TEMPO_StaticTrigTable, elem->trig->func), elem->trig->para);
        }
        DEBUG_SendMessageL("    dst: %s\n", SDL_GetStringFromFRect(elem->dst_rect));
    }

    switch (elem->type) {
        case ELEM_TYPE_FILE:
        case ELEM_TYPE_TEXT:
        case ELEM_TYPE_SWITCH: {
            if (mouseLeftIn && mouseIn && TEMPO_OFEN_RELOAD == false) {
                DEVICE_SetMouseLeftTrig(elem->trig);
            }
            break;
        }
        case ELEM_TYPE_SLID_I:
        case ELEM_TYPE_SLID_F: {
            if (mouseLeftIn == false) {
                break;
            }
            const float min = elem->dst_rect.x + A + B;
            const float max = elem->dst_rect.x + elem->dst_rect.w - A - B;
            const float now = DEVICE_GetMousePos().x;
            if (elem->type == ELEM_TYPE_SLID_F) {
                if (now <= min)
                    *elem->info.slidF.now = elem->info.slidF.min;
                else if (now >= max)
                    *elem->info.slidF.now = elem->info.slidF.max;
                else
                    *elem->info.slidF.now = elem->info.slidF.min + (elem->info.slidF.max - elem->info.slidF.min) * (now - min) / (max - min);
            }
            if (elem->type == ELEM_TYPE_SLID_I) {
                if (now <= min)
                    *elem->info.slidI.now = elem->info.slidI.min;
                else if (now >= max)
                    *elem->info.slidI.now = elem->info.slidI.max;
                else
                    *elem->info.slidI.now = (int)((now - min) / (B + C));
            }
            break;
        }
        default: break;
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
void TEMPO_TrigFuncSwitch(const char* key) {
    bool* val = TABLE_GetValByKey(TEMPO_ExternTable[JSM_BOOL], key);
    // bool* val = BASIC_GetValByKey(TEMPO_PUBLIC_BOOL_LEN, TEMPO_PUBLIC_BOOL, key);
    if (val != NULL) {
        *val = !*val;
    }
}