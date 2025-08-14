#include "elem.h"


// OTHER ===============================================================================================================
static const SDL_FRect ELEM_DEFAULT_GID_RECT = {0, 0, 1, 1};


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
static const char* TEMPO_GetStringFromElemState(const ElemState state) {
    // Req Condition
    if (state >= ELEM_NUM_STATES) {
        printf("%s: state is illegal.\n", __func__);
        return NULL;
    }

    //
    return ELEM_STATE_STRING_SET[state];
}


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
static const char* TEMPO_GetStringFromElemType(const ElemType type) {
    if (type >= ELEM_NUM_TYPES) {
        printf("%s: type is illegal.\n", __func__);
        return NULL;
    }
    return ELEM_TYPE_STRING_SET[type];
}


// ELEM ================================================================================================================
struct Elem {
    // read
    ElemType type;
    char* info;

    int anchor;
    SDL_FRect* gid_rect;
    SDL_FRect* src_rect;

    Trig* trig;

    // renew
    SDL_FRect dst_rect;
    ElemState state;
    bool visible;
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
bool TEMPO_GetElemDstRect(const Elem *elem, SDL_FRect *dst_rect) {
    if (elem == NULL) {
        printf("%s: elem is null.\n", __func__);
        return false;
    }
    *dst_rect = elem->dst_rect;
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
    if (toml_string_in(tomlElem, key = "info").ok) {
        elem->info = strdup(toml_string_in(tomlElem, key).u.s);
        if (elem->info == NULL) {
            printf("%s: failed in %s.\n", __func__, key);
            return false;
        }
    } // string
    if (toml_int_in(tomlElem, key = "anchor").ok) {
        elem->anchor = (int)toml_int_in(tomlElem, key).u.i;
    } // anchor
    if (toml_array_in(tomlElem, key = "gid") != NULL) {
        elem->gid_rect = malloc(sizeof(SDL_FRect));
        if (elem->gid_rect == NULL) {
            printf("%s: elem->gid_rect == NULL in %s.\n", __func__, key);
            return false;
        } // Req Condition

        const bool ok = loadFRectFromTomlArray(elem->gid_rect, toml_array_in(tomlElem, key));
        if (ok == false) {
            printf("%s: ok == false in %s.\n", __func__, key);
            return false;
        } // Req Condition
    } // guide
    if (toml_string_in(tomlElem, key = "func").ok) {
        elem->trig = TEMPO_CreateTrig(tomlElem);
        if (elem->trig == NULL) {
            printf("%s: elem->trig == NULL.\n", __func__);
            return false;
        }
    }
    if (toml_array_in(tomlElem, key = "src") != NULL) {
        elem->src_rect = malloc(sizeof(SDL_FRect));
        if (elem->src_rect == NULL) {
            printf("%s: elem->src_rect == NULL in %s.\n", __func__, key);
            return false;
        } // Req Condition

        const bool ok = loadFRectFromTomlArray(elem->src_rect, toml_array_in(tomlElem, key));
        if (ok == false) {
            printf("%s: ok == false in %s.\n", __func__, key);
            return false;
        } // Req Condition
    } // guide

    return true;
}
static bool TEMPO_CreateElem_CK(const Elem* elem) {
    if (elem->info == NULL) {
        printf("%s: info == NULL.\n", __func__);
        return false;
    } // Req Condition
    return true;
}
Elem* TEMPO_DeleteElem(Elem *elem) {
    {
        if (elem == NULL) {
            return elem;
        } // Opt Condition

        if (elem->info != NULL) {
            free(elem->info); // free
            elem->info = NULL;
        }
        if (elem->trig != NULL) {
            elem->trig = TEMPO_DeleteTrig(elem->trig);
        }
        if (elem->gid_rect != NULL) {
            free(elem->gid_rect);
            elem->gid_rect = NULL;
        }
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
    const bool mouseIn = DEVICE_MouseInRect(&elem->dst_rect);
    const bool mouseLeftIn = DEVICE_MouseLeftInRect(&elem->dst_rect);
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
        DEBUG_SendMessageL("    type: %s\n", TEMPO_GetStringFromElemType(elem->type));
        DEBUG_SendMessageL("    info: %s\n", elem->info);
        DEBUG_SendMessageL("    state: %s\n", TEMPO_GetStringFromElemState(elem->state));
        if (elem->trig != NULL) {
            DEBUG_SendMessageL("    trig: %s(%s)\n", TRIG_GetNameFromFunc(elem->trig->func), elem->trig->para);
        }
    }
}
bool TEMPO_RenewElem(Elem *elem) {
    if (elem->visible == false) {
        return true;
    }
    TEMPO_RenewElemState(elem);
    return true;
}


// DRAW ================================================================================================================
static bool TEMPO_DrawElem_(Elem *elem) {
    // Req Condition
    SDL_Texture* texture = TEMPO_CreateElem_Texture(elem->type, elem->info);
    if (texture == NULL) {
        return false;
    }
    const bool ok = SDL_LoadDstRectAligned(
        &elem->dst_rect,
        texture,
        elem->src_rect,
        elem->gid_rect,
        &basic.bck_rect,
        elem->anchor
        );
    if (ok == false) {
        SDL_DestroyTexture(texture);
        return false;
    }
    switch (elem->state) {
        case ELEM_STATE_PRESSED: {
            DEBUG_FillRect(&elem->dst_rect);
            break;
        }
        default: break;
    }
    SDL_RenderTexture(basic.renderer, texture, elem->src_rect, &elem->dst_rect);
    switch (elem->state) {
        case ELEM_STATE_PRESSED:
        case ELEM_STATE_RELEASE:
        case ELEM_STATE_INSIDE: {
            DEBUG_DrawRect(&elem->dst_rect);
            break;
        }
        default: break;
    }
    SDL_DestroyTexture(texture);
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