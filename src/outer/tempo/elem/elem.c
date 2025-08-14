#include "elem.h"


// ELEM INFO ===========================================================================================================
union ElemInfo {
    char* string;
    struct {} knob;
    struct {int min, max, now;} slid;
};


// ELEM STATE ==========================================================================================================
enum ElemState {
    ELEM_STATE_OUTSIDE,
    ELEM_STATE_INSIDE,
    ELEM_STATE_PRESSED,
    ELEM_STATE_RELEASE,
    ELEM_NUM_STATES
};
static const char* ELEM_STATE_STRING_SET[ELEM_NUM_STATES] = {
    [ELEM_STATE_OUTSIDE] = "OUTSIDE",
    [ELEM_STATE_INSIDE] = "INSIDE",
    [ELEM_STATE_PRESSED] = "PRESSED",
    [ELEM_STATE_RELEASE] = "RELEASE",
};


// ELEM TYPE ===========================================================================================================
enum ElemType {
    ELEM_TYPE_NULL,
    ELEM_TYPE_FILE,
    ELEM_TYPE_TEXT,
    ELEM_TYPE_KNOB,
    ELEM_TYPE_SLID,
    ELEM_NUM_TYPES,
};
static const char* ELEM_TYPE_STRING_SET[ELEM_NUM_TYPES] = {
    [ELEM_TYPE_NULL] = "NULL",
    [ELEM_TYPE_FILE] = "FILE",
    [ELEM_TYPE_TEXT] = "TEXT",
    [ELEM_TYPE_KNOB] = "KNOB",
    [ELEM_TYPE_SLID] = "SLID",
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

    // renew
    SDL_FRect dst;
    ElemState state;
    bool visible;
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
            texture = IMG_LoadTexture(basic.renderer, string); // malloc

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
                ); // malloc
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
        case ELEM_TYPE_SLID: {
            const toml_table_t* tomlInfo = toml_table_in(tomlElem, key);
            if (tomlInfo == NULL) {
                printf("%s: failed in %s.\n", __func__, key);
                return false;
            } // Req Condition
            const toml_datum_t min = toml_int_in(tomlInfo, "min");
            const toml_datum_t max = toml_int_in(tomlInfo, "max");
            const toml_datum_t now = toml_int_in(tomlInfo, "now");
            if (min.ok && max.ok && now.ok) {
                elem->info.slid.min = (int)min.u.i;
                elem->info.slid.max = (int)max.u.i;
                elem->info.slid.now = (int)now.u.i;
            }
            else {
                printf("%s: failed in %s.\n", __func__, key);
                return false;
            }
        }
        default: break;
    }
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
    if (toml_string_in(tomlElem, key = "func").ok) {
        elem->trig = TEMPO_CreateTrig(tomlElem);
        if (elem->trig == NULL) {
            printf("%s: elem->trig == NULL, %s.\n", __func__, key);
            return false;
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
                free(elem->info.string); // free
                elem->info.string = NULL;
            }
            break;
        }
        default: break;
    }
    if (elem->trig != NULL) {
        elem->trig = TEMPO_DeleteTrig(elem->trig);
    }
    free(elem); // free
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
static void TEMPO_RenewElemState(Elem* elem) {
    const bool mouseIn = DEVICE_MouseInRect(&elem->dst);
    const bool mouseLeftIn = DEVICE_MouseLeftInRect(&elem->dst);
    if (elem->state == ELEM_STATE_PRESSED && mouseIn == true && mouseLeftIn == false) {
        elem->state = ELEM_STATE_RELEASE;
    }
    else {
        if (mouseIn == true) {
            elem->state = mouseLeftIn ? ELEM_STATE_PRESSED : ELEM_STATE_INSIDE;
        }
        else {
            elem->state = ELEM_STATE_OUTSIDE;
        }
    }
    if (elem->state == ELEM_STATE_RELEASE && elem->trig != NULL) {
        elem->trig->func(elem->trig->para);
    }

    if (elem->state == ELEM_STATE_PRESSED) {
        DEBUG_SendMessageL("Elem:\n");
        DEBUG_SendMessageL("    type: %s\n", ELEM_TYPE_STRING_SET[elem->type]);
        // DEBUG_SendMessageL("    info: %s\n", elem->info);
        DEBUG_SendMessageL("    state: %s\n", ELEM_STATE_STRING_SET[elem->state]);
        if (elem->trig != NULL) {
            DEBUG_SendMessageL("    trig: %s(%s)\n", TRIG_GetNameFromFunc(elem->trig->func), elem->trig->para);
        }
    }
}
bool TEMPO_RenewElem(Elem *elem) {
    if (elem->visible == false) {
        // return false;
    }
    TEMPO_RenewElemState(elem);
    return true;
}


// DRAW ================================================================================================================
static bool TEMPO_DrawElem_DstRect(Elem *elem) {
    return SDL_LoadDstRectAligned(
        &elem->dst,
        NULL,
        &elem->src,
        &elem->gid,
        &basic.bck,
        elem->anchor
        );
}
static bool TEMPO_DrawElem_(Elem* elem) {
    switch (elem->state) {
        case ELEM_STATE_PRESSED: {
            DEBUG_FillRect(&elem->dst);
            break;
        }
        default: break;
    }
    switch (elem->type) {
        case ELEM_TYPE_TEXT: {}
        case ELEM_TYPE_FILE: {
            SDL_Texture* texture = TEMPO_CreateElem_Texture(elem->type, elem->info.string);
            if (texture == NULL) {
                return false;
            }
            TEMPO_DrawElem_DstRect(elem);
            SDL_RenderTexture(basic.renderer, texture, &elem->src, &elem->dst);
            SDL_DestroyTexture(texture);
            break;
        }
        case ELEM_TYPE_SLID: {
            const float A = 4, B = 4, C = 36, D = 64;
            const float M = (float)(elem->info.slid.max - elem->info.slid.min);
            const int N = elem->info.slid.now - elem->info.slid.min;
            elem->src.x = elem->src.y = 0;
            const float W = elem->src.w = 2 * A + (M + 1) * B + M * C;
            const float H = elem->src.h = 2 * A + 2 * B + D;
            elem->gid.w = elem->gid.h = 1;
            TEMPO_DrawElem_DstRect(elem);
            const float X = elem->dst.x, Y = elem->dst.y;
            SDL_SetRenderDrawColor(basic.renderer, 255, 255, 200, 255);
            const SDL_FRect frame[4] = {
                (SDL_FRect){X, Y, W, A},
                (SDL_FRect){X, Y, A, H},
                (SDL_FRect){X, Y + H - A, W, A},
                (SDL_FRect){X + W - A, Y, A, H},
            };
            SDL_RenderFillRects(basic.renderer, frame, 4);
            SDL_FRect rects[N];
            for (int i = 0; i < N; i++) {
                rects[i].x = X + A + (float)(i + 1) * B + (float)i * C;
                rects[i].y = Y + A + B;
                rects[i].w = C;
                rects[i].h = D;
            }
            SDL_RenderFillRects(basic.renderer, rects, N);
            break;
        }
        case ELEM_TYPE_KNOB: {
            const float A = 4, B = 4, C = 36;
            elem->src = (SDL_FRect){0, 0, 2 * C, 2 * C};
            TEMPO_DrawElem_DstRect(elem);
            const float X = elem->dst.x, Y = elem->dst.y;

        }
        default: break;
    }
    switch (elem->state) {
        case ELEM_STATE_PRESSED:
        case ELEM_STATE_RELEASE:
        case ELEM_STATE_INSIDE: {
            DEBUG_DrawRect(&elem->dst);
            break;
        }
        default: break;
    }
    return true;
}
bool TEMPO_DrawElem(Elem *elem) {
    // Req Condition
    if (basic.renderer == NULL) {
        DEBUG_SendMessageR("%s: menu.renderer is NULL.\n", __func__);
        return false;
    }
    return elem->visible = TEMPO_DrawElem_(elem);
}