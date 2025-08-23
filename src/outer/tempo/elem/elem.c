#include "elem.h"

typedef enum ElemType ElemType;
typedef union ElemInfo ElemInfo;

// ELEM INFO ===========================================================================================================
union ElemInfo {
    char* string;
    struct SlidI {int min, max, *now;} slidI;
    struct SlidF {float min, max, *now;} slidF;
    struct Switch {bool* now;} switch_;
};


// ELEM TYPE ===========================================================================================================
enum ElemType {
    ELEM_TYPE_NULL,
    ELEM_TYPE_FILE,
    ELEM_TYPE_TEXT,
    ELEM_TYPE_SLID_F,
    ELEM_TYPE_SLID_I,
    ELEM_TYPE_SWITCH,
    ELEM_NUM_TYPES,
};
const char* ELEM_TYPE_STRING_SET[ELEM_NUM_TYPES] = {
    [ELEM_TYPE_NULL] = "NULL",
    [ELEM_TYPE_FILE] = "FILE",
    [ELEM_TYPE_TEXT] = "TEXT",
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


// ELEM ================================================================================================================
struct Elem {
    // read
    ElemType type;
    ElemInfo info;

    int anchor;
    SDL_FRect gid;
    SDL_FRect src;

    Trig* trig;

    SDL_Texture* tex;

    // renew
    SDL_FRect dst;
};


// SET & GET ===========================================================================================================
static bool TEMPO_SetElemDstRect(Elem *elem, const SDL_FRect dst_rect) {
    if (elem == NULL) {
        printf("%s: elem is null.\n", __func__);
        return false;
    }
    elem->dst = dst_rect;
    return true;
}
static bool TEMPO_GetElemDstRect(const Elem *elem, SDL_FRect *dst_rect) {
    if (elem == NULL) {
        printf("%s: elem is null.\n", __func__);
        return false;
    }
    *dst_rect = elem->dst;
    return true;
}


// CREATE & DELETE =====================================================================================================
static SDL_Texture* TEMPO_CreateElem_Texture(const ElemType type, const char* string) {
    SDL_Texture* texture = NULL;
    switch (type) {
        case ELEM_TYPE_FILE: {
            texture = IMG_LoadTexture(renderer, string);

            if (texture == NULL) {
                printf("%s: failed from \"%s\".\n", __func__, string);
                return NULL;
            } // Req Condition
            SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);
            break;
        }
        case ELEM_TYPE_TEXT: {
            texture = TXT_LoadTextureWithLines(
                renderer,
                theme.font,
                string,
                (SDL_Color){255, 255, 255, 255},
                EMPTY,
                'C'
                );
            if (texture == NULL) {
                printf("%s: failed from \"%s\".\n", __func__, string);
                return NULL;
            } // Req Condition
            SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);
            break;
        }
        default: {
            texture = NULL;
            break;
        }
    }
    return texture;
}
static bool TEMPO_CreateElem_RK(Elem* elem, const cJSON *elem_json) {
    memset(elem, 0, sizeof(Elem));

    const char* key;

    // object, key: string -> int
    const cJSON* type_json = cJSON_GetObjectItem(elem_json, key = "type");
    if (type_json != NULL && cJSON_IsString(type_json)) {
        elem->type = TEMPO_GetElemTypeFromString(type_json->valuestring);
        if (elem->type == ELEM_TYPE_NULL) {
            printf("%s: failed in %s.\n", __func__, key);
            return false;
        } // Req Condition
    } // type

    const cJSON* info_json = cJSON_GetObjectItem(elem_json, key="info");
    if (info_json == NULL) {
        printf("%s: failed in %s.\n", __func__, key);
        return false;
    } // Req Condition
    switch (elem->type) {
        case ELEM_TYPE_TEXT: {}
        case ELEM_TYPE_FILE: {
            if (cJSON_Load(elem_json, "info", STRING, &elem->info.string) == false) {
                printf("%s: failed in %s.\n", __func__, key);
                return false;
            } // Req Condition
            break;
        }
        case ELEM_TYPE_SLID_I: {}
        case ELEM_TYPE_SLID_F: {
            if (cJSON_IsObject(info_json) == false) {
                printf("%s: failed in %s.\n", __func__, key);
                return false;
            }
            // cJSON_Load(info_json, "min", INT, &elem->info.slidI.min);
            const cJSON* min_json = cJSON_GetObjectItem(info_json, "min");
            const cJSON* max_json = cJSON_GetObjectItem(info_json, "max");
            const cJSON* now_json = cJSON_GetObjectItem(info_json, "now");
            if (cJSON_IsNumber(min_json) && cJSON_IsNumber(max_json) && cJSON_IsString(now_json)) {
                if (elem->type == ELEM_TYPE_SLID_I) {
                    elem->info.slidI.min = min_json->valueint;
                    elem->info.slidI.max = max_json->valueint;
                    elem->info.slidI.now = BASIC_GetValByKey(TEMPO_TABLE_INT, now_json->valuestring);
                }
                else if (elem->type == ELEM_TYPE_SLID_F) {
                    elem->info.slidF.min = (float)min_json->valuedouble;
                    elem->info.slidF.max = (float)max_json->valuedouble;
                    elem->info.slidF.now = BASIC_GetValByKey(TEMPO_TABLE_FLOAT, now_json->valuestring);
                }
            }
            else {
                printf("%s: failed in %s.\n", __func__, key);
                return false;
            }


            break;
        }
        case ELEM_TYPE_SWITCH: {
            elem->info.switch_.now = BASIC_GetValByKey(TEMPO_TABLE_BOOL, info_json->valuestring);
            elem->trig = CreateTrig(TEMPO_TrigFuncSwitch, info_json->valuestring);
            if (elem->trig == NULL) {
                printf("%s: failed in %s.\n", __func__, key);
                return false;
            }
            break;
        }
        default: break;
    } // info

    if (cJSON_Load(elem_json, key = "anchor", INT, &elem->anchor) == false) {
       printf("%s: failed in %s.\n", __func__, key);
       return false;
   } // Req Condition
    elem->gid = (SDL_FRect){0, 0, 1, 1};
    if (cJSON_Load(elem_json, key = "gid", RECT, &elem->gid) == false) {
        printf("%s: failed in %s.\n", __func__, key);
        return false;
    } // Req Condition

    // object, key: string -> ptr
    {
        TrigFunc func = NULL;
        const char* para = NULL;

        const cJSON* func_json = cJSON_GetObjectItem(elem_json, "func");
        if (func_json != NULL && cJSON_IsString(func_json)) {
            func = BASIC_GetValByKey(TEMPO_MENU_TRIG_SET, func_json->valuestring);
            if (func == NULL) {
                printf("%s: func == NULL, %s.\n", __func__, key);
                return false;
            }
        } // func

        const cJSON* para_json = cJSON_GetObjectItem(elem_json, "para");
        if (para_json != NULL && cJSON_IsString(para_json)) {
            para = para_json->valuestring;
            if (para == NULL) {
                printf("%s: para == NULL, %s.\n", __func__, key);
                return false;
            }
        }
        if (func != NULL) {
            elem->trig = CreateTrig(func, para);
            if (elem->trig == NULL) {
                printf("%s: failed in %s.\n", __func__, key);
                return false;
            }
        }
    }

    /*if (toml_array_in(elem_json, key = "src") != NULL) {
        const bool ok = loadFRectFromTomlArray(&elem->src, toml_array_in(elem_json, key));
        if (ok == false) {
            printf("%s: ok == false in %s.\n", __func__, key);
            return false;
        } // Req Condition
    } // src*/
    if (elem->type == ELEM_TYPE_TEXT || elem->type == ELEM_TYPE_FILE) {
        SDL_Texture* texture = TEMPO_CreateElem_Texture(elem->type, elem->info.string);
        SDL_GetTextureSize(texture, &elem->src.w, &elem->src.h);
        SDL_DestroyTexture(texture);
        texture = NULL;
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
const float A = 4, B = 4, C = 8, D = 48;

static bool TEMPO_RenewElemTex(Elem* elem) {
    if (elem->tex != NULL) {
        SDL_DestroyTexture(elem->tex);
        elem->tex = NULL;
    }
    switch (elem->type) {
        case ELEM_TYPE_TEXT:
        case ELEM_TYPE_FILE: {
            elem->tex = TEMPO_CreateElem_Texture(elem->type, elem->info.string);
            if (elem->tex == NULL) {
                return false;
            }
            break;
        }
        case ELEM_TYPE_SLID_I:
        case ELEM_TYPE_SLID_F: {
            const float M = elem->type == ELEM_TYPE_SLID_I
                    ? (float)(elem->info.slidI.max - elem->info.slidI.min)
                    : elem->info.slidF.max - elem->info.slidF.min;
            const float W = 2 * A + (M + 1) * B + M * C;
            const float H = 2 * A + 2 * B + D;
            elem->src = (SDL_FRect){0, 0, W, H};
            elem->gid.w = elem->gid.h = 1;
            elem->tex = SDL_CreateTexture(
                renderer,
                SDL_PIXELFORMAT_RGBA8888,
                SDL_TEXTUREACCESS_TARGET,
                (int)W, (int)H);
            SDL_SetRenderTarget(renderer, elem->tex);
            {
                SDL_SetRenderDrawColor(renderer, 255, 255, 200, 255);
                const SDL_FRect frame[4] = {
                    (SDL_FRect){0, 0, W, A},
                    (SDL_FRect){0, 0, A, H},
                    (SDL_FRect){0, H - A, W, A},
                    (SDL_FRect){W - A, 0, A, H},
                };
                SDL_RenderFillRects(renderer, frame, 4);
            }
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
            const float A = 4, B = 4, C = 48, D = 48;
            const float M = 1;
            const float N = *elem->info.switch_.now;
            const float W = 2 * A + (M + 1) * B + M * C;
            const float H = 2 * A + 2 * B + D;
            elem->src = (SDL_FRect){0, 0, W, H};
            elem->gid.w = elem->gid.h = 1;
            elem->tex = SDL_CreateTexture(
                renderer,
                SDL_PIXELFORMAT_RGBA8888,
                SDL_TEXTUREACCESS_TARGET,
                (int)W, (int)H);
            SDL_SetRenderTarget(renderer, elem->tex);
            {
                SDL_SetRenderDrawColor(renderer, 255, 255, 200, 255);
                const SDL_FRect frame[5] = {
                    (SDL_FRect){0, 0, W, A},
                    (SDL_FRect){0, 0, A, H},
                    (SDL_FRect){0, H - A, W, A},
                    (SDL_FRect){W - A, 0, A, H},
                    (SDL_FRect){A + B, A + B, (W - 2 * A - 2 * B) * N / M, H - 2 * A - 2 * B},
                };
                SDL_RenderFillRects(renderer, frame, 5);
            }
            SDL_SetRenderTarget(renderer, NULL);
            break;
        }
        default: break;
    }
    return true;
}
static bool TEMPO_RenewElemDstRect(Elem *elem) {
    const bool result = SDL_LoadDstRectAligned(
        &elem->dst,
        elem->tex,
        &elem->src,
        &elem->gid,
        NULL,
        elem->anchor
        );
    return result;
}
static bool TEMPO_RenewElemState(const Elem* elem) {
    const bool mouseIn = DEVICE_MouseInRect(&elem->dst);
    const bool mouseLeftIn = DEVICE_MouseLeftInRect(&elem->dst);

    if (mouseLeftIn && mouseIn) {
        DEVICE_SetMouseLeftTrig(elem->trig);
    }
    if (mouseLeftIn) {
        DEBUG_SendMessageL("Elem:\n");
        DEBUG_SendMessageL("    type: %s\n", ELEM_TYPE_STRING_SET[elem->type]);
        // DEBUG_SendMessageL("    info: %s\n", elem->info);
        if (elem->trig != NULL) {
            DEBUG_SendMessageL("    trig: %s(%s)\n", BASIC_GetKeyByVal(TEMPO_MENU_TRIG_SET, elem->trig->func), elem->trig->para);
        }
    }
    if (mouseLeftIn) {
        const float min = elem->dst.x + A + B;
        const float max = elem->dst.x + elem->dst.w - A - B;
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
    }
    return true;
}
static bool TEMPO_RenewElemInfo(Elem* elem) {
    return true;
}
bool TEMPO_RenewElem(Elem *elem) {
    if (TEMPO_RenewElemTex(elem) == false) {
        printf("%s: TEMPO_RenewElemTex(elem) == false\n", __func__);
        return false;
    }
    if (TEMPO_RenewElemDstRect(elem) == false) {
        printf("%s: TEMPO_RenewElemDstRect(elem) == false\n", __func__);
        return false;
    }
    if (TEMPO_RenewElemState(elem) == false) {
        return false;
    }
    if (TEMPO_RenewElemInfo(elem) == false) {
        return false;
    }
    return true;
}


// DRAW ================================================================================================================
static bool TEMPO_DrawElem_(const Elem* elem) {
    const bool mouseIn = DEVICE_MouseInRect(&elem->dst);
    const bool mouseLeftIn = DEVICE_MouseLeftInRect(&elem->dst);
    if (mouseLeftIn) {
        DEBUG_FillRect(&elem->dst);
    }
    SDL_RenderTexture(renderer, elem->tex, &elem->src, &elem->dst);
    if (mouseIn || mouseLeftIn) {
        DEBUG_DrawRect(&elem->dst);
    }
    return true;
}
bool TEMPO_DrawElem(const Elem *elem) {
    // Req Condition
    if (renderer == NULL) {
        DEBUG_SendMessageR("%s: menu.renderer is NULL.\n", __func__);
        return false;
    }
    if (TEMPO_DrawElem_(elem) == false) {
        DEBUG_SendMessageR("%s: ???.\n", __func__);
        return false;
    }
    return true;
}


// TRIG ================================================================================================================
void TEMPO_TrigFuncSwitch(const char* key) {
    bool* val = BASIC_GetValByKey(TEMPO_TABLE_BOOL, key);
    if (val != NULL) {
        *val = !*val;
    }
}