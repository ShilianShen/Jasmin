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
static const char* ELEM_STATE_STRINGS[ELEM_NUM_STATES] = {
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
    return ELEM_STATE_STRINGS[state];
}


// ELEM TYPE ===========================================================================================================
static const char* ELEM_TYPE_STRINGS[ELEM_NUM_TYPES] = {
    [ELEM_TYPE_NULL] = "NULL",
    [ELEM_TYPE_FILE] = "FILE",
    [ELEM_TYPE_TEXT] = "TEXT"
};
static ElemType TEMPO_GetElemTypeFromString(const char* string) {
    for (int i = 0; i < ELEM_NUM_TYPES; i++) {
        if (strcmp(string, ELEM_TYPE_STRINGS[i]) == 0) {
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
    return ELEM_TYPE_STRINGS[type];
}


// ELEM ================================================================================================================
struct Elem {
    // read
    ElemType type;
    char* string;
    int anchor;
    SDL_FRect gid_rect;
    TrigFunc trig_func;
    char* trig_para;

    // create
    SDL_Texture* texture; SDL_FRect src_rect;

    // renew
    SDL_FRect dst_rect;
    ElemState state;
    bool visible;
};


// SET & GET ===========================================================================================================
void TEMPO_SetElemGidRect(Elem* elem, const SDL_FRect gid_rect) {
    if (elem == NULL) {
        printf("%s: elem is null.\n", __func__);
        return;
    }
    elem->gid_rect = gid_rect;
}
void TEMPO_SetElemDstRect(Elem* elem, const SDL_FRect dst_rect) {
    if (elem == NULL) {
        printf("%s: elem is null.\n", __func__);
        return;
    }
    elem->dst_rect = dst_rect;
}
void TEMPO_GetElemDstRect(const Elem* elem, SDL_FRect* dst_rect) {
    if (elem == NULL) {
        printf("%s: elem is null.\n", __func__);
        return;
    }
    *dst_rect = elem->dst_rect;
}


// CHECK ===============================================================================================================
static bool TEMPO_CheckElem(const Elem* elem) {
    return elem != NULL && elem->string != NULL && elem->texture != NULL;
}


// CREATE & DELETE =====================================================================================================
static SDL_Texture* TEMPO_CreateElem_Texture(const ElemType type, const char* string) {
    SDL_Texture* texture = NULL;
    switch (type) {
        case ELEM_TYPE_FILE: {
            texture = IMG_LoadTexture(basic.renderer, string); // malloc
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
            break;
        }
        default: {
            texture = NULL;
            break;
        }
    }
    if (texture == NULL) {
        printf("%s: failed from \"%s\".\n", __func__, string);
        return NULL;
    } // Req Condition
    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);
    return texture;
}
void TEMPO_DeleteElem(Elem* elem) {
    {
        if (elem == NULL) {
            return;
        } // Opt Condition

        if (elem->string != NULL) {
            free(elem->string); // free
            elem->string = NULL;
        }
        if (elem->trig_para != NULL) {
            free(elem->trig_para); // free
            elem->trig_para = NULL;
        }
        if (elem->texture != NULL) {
            SDL_DestroyTexture(elem->texture); // free
            elem->texture = NULL;
        }
    }
    free(elem); // free
    elem = NULL;
}
static bool TEMPO_CreateElem_(Elem* elem, const toml_table_t *tomlInfo) {
    memset(elem, 0, sizeof(Elem));

    const char* key;
    if (toml_string_in(tomlInfo, key = "type").ok) {
        elem->type = TEMPO_GetElemTypeFromString(toml_string_in(tomlInfo, key).u.s);
        if (elem->type == ELEM_TYPE_NULL) {
            printf("%s: failed in %s.\n", __func__, key);
            return false;
        }
    } // type
    if (toml_string_in(tomlInfo, key = "info").ok) {
        elem->string = strdup(toml_string_in(tomlInfo, key).u.s);
        if (elem->string == NULL) {
            printf("%s: failed in %s.\n", __func__, key);
            return false;
        }
    } // string
    if (toml_int_in(tomlInfo, key = "anchor").ok) {
        elem->anchor = (int)toml_int_in(tomlInfo, key).u.i;
    } // anchor
    if (toml_array_in(tomlInfo, key = "guide") != NULL) {
        const bool ok = loadFRectFromTomlArray(&elem->gid_rect, toml_array_in(tomlInfo, key));
        if (ok == false) {
            printf("%s: failed in %s.\n", __func__, key);
            return false;
        }
    } // guide
    else {
        elem->gid_rect = ELEM_DEFAULT_GID_RECT;
    }
    if (toml_string_in(tomlInfo, key = "func").ok) {
        elem->trig_func = TRIG_FindFuncFromName(toml_string_in(tomlInfo, key).u.s);
        if (elem->trig_func == NULL) {
            printf("%s: failed in %s.\n", __func__, key);
            return false;
        }
    } // trig_func
    if (toml_string_in(tomlInfo, key = "para").ok) {
        elem->trig_para = strdup(toml_string_in(tomlInfo, key).u.s);
        if (elem->trig_para == NULL) {
            printf("%s: failed in %s.\n", __func__, key);
            return false;
        }
    } // trig_para
    {
        elem->texture = TEMPO_CreateElem_Texture(elem->type, elem->string);
        if (elem->texture == NULL) {
            printf("%s: failed in %s.\n", __func__, "texture");
            return false;
        }
        float w, h;
        SDL_GetTextureSize(elem->texture, &w, &h);
        elem->src_rect = (SDL_FRect){0, 0, w, h};
    } // texture

    return true;
}
Elem* TEMPO_CreateElem(const toml_table_t *tomlInfo) {
    if (tomlInfo == NULL) {return NULL;}

    Elem* elem = malloc(sizeof(Elem));
    if (elem == NULL) {return NULL;}

    if (TEMPO_CreateElem_(elem, tomlInfo) == false) {
        TEMPO_DeleteElem(elem);
        elem = NULL;
    }
    return elem;
}


// RENEW ===============================================================================================================
static void TEMPO_RenewElemDstRect(Elem* elem) {
    SDL_LoadDstRectAligned(
        &elem->dst_rect,
        elem->texture,
        &elem->src_rect,
        &elem->gid_rect,
        &basic.bck_rect,
        elem->anchor
        );
}
static void TEMPO_RenewElemState(Elem* elem) {
    if (elem->visible == false) {
        return;
    }
    const bool mouseIn = DEVICE_MouseInRect(&elem->dst_rect);
    const bool mouseLeftIn = DEVICE_MouseLeftInRect(&elem->dst_rect);
    if (elem->state == ELEM_STATE_PRESSED) {
        DEBUG_SendMessageL("elem: %s, %s\n", TEMPO_GetStringFromElemType(elem->type), elem->string);
        DEBUG_SendMessageL("elem.state: %s\n", TEMPO_GetStringFromElemState(elem->state));
        if (elem->trig_func != NULL) {
            DEBUG_SendMessageL("elem.trig: %s, %s\n", TRIG_FindNameFromFunc(elem->trig_func), elem->trig_para);
        }
    }
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
    if (elem->state == ELEM_STATE_RELEASE && elem->trig_func != NULL) {
        elem->trig_func(elem->trig_para);
    }
}
void TEMPO_RenewElem(Elem* elem) {
    // Req Condition
    TEMPO_RenewElemState(elem);
    elem->visible = false;
    TEMPO_RenewElemDstRect(elem);
}


// DRAW ================================================================================================================
static bool TEMPO_DrawElem_(const Elem* elem) {
    // Req Condition

    //
    switch (elem->state) {
        case ELEM_STATE_PRESSED: {
            DEBUG_FillRect(&elem->dst_rect);
            SDL_RenderTexture(basic.renderer, elem->texture, &elem->src_rect, &elem->dst_rect);
            DEBUG_DrawRect(&elem->dst_rect);
            break;
        }
        case ELEM_STATE_RELEASE:
        case ELEM_STATE_INSIDE: {
            SDL_RenderTexture(basic.renderer, elem->texture, &elem->src_rect, &elem->dst_rect);
            DEBUG_DrawRect(&elem->dst_rect);
            break;
        }
        case ELEM_STATE_OUTSIDE: {
            SDL_RenderTexture(basic.renderer, elem->texture, &elem->src_rect, &elem->dst_rect);
            break;
        }
        default: break;
    }
    return true;
}
void TEMPO_DrawElem(Elem *elem) {
    // Req Condition
    elem->visible = false;
    if (basic.renderer == NULL) {
        printf("%s: menu.renderer is NULL.\n", __func__);
        return;
    }
    elem->visible = TEMPO_DrawElem_(elem);
}