#include "elem.h"


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
ElemType TEMPO_GetElemTypeFromString(const char* string) {
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
bool TEMPO_SetElemDstRect(Elem *elem, const SDL_FRect dst_rect) {
    if (elem == NULL) {
        printf("%s: elem is null.\n", __func__);
        return false;
    }
    elem->dst = dst_rect;
    return true;
}
bool TEMPO_GetElemDstRect(const Elem *elem, SDL_FRect *dst_rect) {
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
            texture = IMG_LoadTexture(basic.renderer, string);

            if (texture == NULL) {
                printf("%s: failed from \"%s\".\n", __func__, string);
                return NULL;
            } // Req Condition
            SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);
            break;
        }
        case ELEM_TYPE_TEXT: {
            texture = TXT_LoadTextureWithLines(
                basic.renderer,
                basic.font,
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
static bool TEMPO_CreateElem_RK(Elem* elem, const toml_table_t *tomlElem) {
    memset(elem, 0, sizeof(Elem));

    const char* key;
    if (toml_string_in(tomlElem, key = "type").ok) {
        elem->type = TEMPO_GetElemTypeFromString(toml_string_in(tomlElem, key).u.s);
        if (elem->type == ELEM_TYPE_NULL) {
            printf("%s: failed in %s.\n", __func__, key);
            return false;
        }
    } // type
    key = "info";
    switch (elem->type) {
        case ELEM_TYPE_TEXT: {}
        case ELEM_TYPE_FILE: {
            if (toml_string_in(tomlElem, key).ok == false) {
                printf("%s: failed in %s.\n", __func__, key);
                return false;
            } // Req Condition
            elem->info.string = strdup(toml_string_in(tomlElem, key).u.s);
            if (elem->info.string == NULL) {
                printf("%s: failed in %s.\n", __func__, key);
                return false;
            } // Req Condition
            break;
        } // string
        case ELEM_TYPE_SLID_I: {
            const toml_table_t* tomlInfo = toml_table_in(tomlElem, key);
            if (tomlInfo == NULL) {
                printf("%s: failed in %s.\n", __func__, key);
                return false;
            } // Req Condition
            const toml_datum_t min = toml_int_in(tomlInfo, "min");
            const toml_datum_t max = toml_int_in(tomlInfo, "max");
            const toml_datum_t now = toml_string_in(tomlInfo, "now");
            if (min.ok && max.ok && now.ok) {
                elem->info.slidI.min = (int)min.u.i;
                elem->info.slidI.max = (int)max.u.i;
                elem->info.slidI.now = BASIC_GetValByKey(TEMPO_TABLE_INT, now.u.s);
            }
            else {
                printf("%s: failed in %s.\n", __func__, key);
                return false;
            }
            break;
        }
        case ELEM_TYPE_SLID_F: {
            const toml_table_t* tomlInfo = toml_table_in(tomlElem, key);
            if (tomlInfo == NULL) {
                printf("%s: failed in %s.\n", __func__, key);
                return false;
            } // Req Condition
            const toml_datum_t min = toml_double_in(tomlInfo, "min");
            const toml_datum_t max = toml_double_in(tomlInfo, "max");
            const toml_datum_t now = toml_string_in(tomlInfo, "now");
            if (min.ok && max.ok && now.ok) {
                elem->info.slidF.min = (float)min.u.d;
                elem->info.slidF.max = (float)max.u.d;
                elem->info.slidF.now = BASIC_GetValByKey(TEMPO_TABLE_FLOAT, now.u.s);
            }
            else {
                printf("%s: failed in %s.\n", __func__, key);
                return false;
            }
            break;
        }
        case ELEM_TYPE_SWITCH: {
            if (toml_string_in(tomlElem, key).ok == false) {
                printf("%s: failed in %s.\n", __func__, key);
                return false;
            }
            elem->info.switch_.now = BASIC_GetValByKey(TEMPO_TABLE_BOOL, toml_string_in(tomlElem, key).u.s);
            const TrigFunc func = TEMPO_TrigFuncSwitch;
            elem->trig = CreateTrig(func, toml_string_in(tomlElem, key).u.s);
            if (elem->trig == NULL) {
                printf("%s: failed in %s.\n", __func__, key);
                return false;
            }
            break;
        }
        default: break;
    } // info
    if (toml_int_in(tomlElem, key = "anchor").ok) {
        elem->anchor = (int)toml_int_in(tomlElem, key).u.i;
    } // anchor
    if (toml_array_in(tomlElem, key = "gid") != NULL) {
        const bool ok = loadFRectFromTomlArray(&elem->gid, toml_array_in(tomlElem, key));
        if (ok == false) {
            printf("%s: ok == false in %s.\n", __func__, key);
            return false;
        } // Req Condition
    } // gid
    else {
        elem->gid = (SDL_FRect){0, 0, 1, 1};
    }
    {
        TrigFunc func = NULL;
        const char* para = NULL;
        if (toml_string_in(tomlElem, key = "func").ok) {
            func = BASIC_GetValByKey(TEMPO_MENU_TRIG_SET, toml_string_in(tomlElem, key).u.s);
            if (func == NULL) {
                printf("%s: func == NULL, %s.\n", __func__, key);
                return false;
            }
        } // func
        if (toml_string_in(tomlElem, key = "para").ok) {
            para = toml_string_in(tomlElem, key).u.s;
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
    if (toml_array_in(tomlElem, key = "src") != NULL) {
        const bool ok = loadFRectFromTomlArray(&elem->src, toml_array_in(tomlElem, key));
        if (ok == false) {
            printf("%s: ok == false in %s.\n", __func__, key);
            return false;
        } // Req Condition
    } // src
    else if (elem->type == ELEM_TYPE_TEXT || elem->type == ELEM_TYPE_FILE) {
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
Elem* TEMPO_CreateElem(const toml_table_t *tomlElem) {
    if (tomlElem == NULL) {
        printf("%s: tomlElem == NULL\n", __func__);
        return NULL;
    } // Req Condition
    Elem* elem = calloc(1, sizeof(Elem));
    if (elem == NULL) {
        printf("%s: elem == NULL.\n", __func__);
        return elem;
    } // Req Condition
    if (TEMPO_CreateElem_RK(elem, tomlElem) == false || TEMPO_CreateElem_CK(elem) == false) {
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
                basic.renderer,
                SDL_PIXELFORMAT_RGBA8888,
                SDL_TEXTUREACCESS_TARGET,
                (int)W, (int)H);
            SDL_SetRenderTarget(basic.renderer, elem->tex);
            {
                SDL_SetRenderDrawColor(basic.renderer, 255, 255, 200, 255);
                const SDL_FRect frame[4] = {
                    (SDL_FRect){0, 0, W, A},
                    (SDL_FRect){0, 0, A, H},
                    (SDL_FRect){0, H - A, W, A},
                    (SDL_FRect){W - A, 0, A, H},
                };
                SDL_RenderFillRects(basic.renderer, frame, 4);
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
                    SDL_RenderFillRects(basic.renderer, rects, N);
                }
                else {
                    SDL_RenderLine(basic.renderer, 0, 0, W, H);
                    SDL_RenderLine(basic.renderer, W, 0, 0, H);
                }
            }
            if (elem->type == ELEM_TYPE_SLID_F) {
                if (elem->info.slidF.now != NULL) {
                    const float N = *elem->info.slidF.now - elem->info.slidF.min;
                    const SDL_FRect rect = {A + B, A + B, (W - 2 * A - 2 * B) * N / M, H - 2 * A - 2 * B};
                    SDL_RenderFillRect(basic.renderer, &rect);
                }
                else {
                    SDL_RenderLine(basic.renderer, 0, 0, W, H);
                    SDL_RenderLine(basic.renderer, W, 0, 0, H);
                }
            }
            SDL_SetRenderTarget(basic.renderer, NULL);
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
                basic.renderer,
                SDL_PIXELFORMAT_RGBA8888,
                SDL_TEXTUREACCESS_TARGET,
                (int)W, (int)H);
            SDL_SetRenderTarget(basic.renderer, elem->tex);
            {
                SDL_SetRenderDrawColor(basic.renderer, 255, 255, 200, 255);
                const SDL_FRect frame[5] = {
                    (SDL_FRect){0, 0, W, A},
                    (SDL_FRect){0, 0, A, H},
                    (SDL_FRect){0, H - A, W, A},
                    (SDL_FRect){W - A, 0, A, H},
                    (SDL_FRect){A + B, A + B, (W - 2 * A - 2 * B) * N / M, H - 2 * A - 2 * B},
                };
                SDL_RenderFillRects(basic.renderer, frame, 5);
            }
            SDL_SetRenderTarget(basic.renderer, NULL);
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
        &basic.bck,
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
    SDL_RenderTexture(basic.renderer, elem->tex, &elem->src, &elem->dst);
    if (mouseIn || mouseLeftIn) {
        DEBUG_DrawRect(&elem->dst);
    }
    return true;
}
bool TEMPO_DrawElem(const Elem *elem) {
    // Req Condition
    if (basic.renderer == NULL) {
        DEBUG_SendMessageR("%s: menu.renderer is NULL.\n", __func__);
        return false;
    }
    if (TEMPO_DrawElem_(elem) == false) {
        DEBUG_SendMessageR("%s: ???.\n", __func__);
        return false;
    }
    return true;
}


void TEMPO_TrigFuncSwitch(const char* key) {
    bool* val = BASIC_GetValByKey(TEMPO_TABLE_BOOL, key);
    if (val != NULL) {
        *val = !*val;
    }
}