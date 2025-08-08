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
static SDL_Texture* TEMPO_CreateElem_Texture(const ElemInfo info) {
    SDL_Texture* texture = NULL;
    switch (info.type) {
        case ELEM_TYPE_FILE: {
            texture = IMG_LoadTexture(basic.renderer, info.string); // malloc
            break;
        }
        case ELEM_TYPE_TEXT: {
            texture = TXT_LoadTextureWithLines(
                basic.renderer,
                basic.font,
                info.string,
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
        printf("%s: failed from \"%s\".\n", __func__, info.string);
        return NULL;
    } // Req Condition
    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);
    return texture;
}
static Elem* TEMPO_CreateElem_(const ElemInfo info) {
    // 这个函数只负责把info的信息填到elem
    Elem* elem = malloc(sizeof(Elem)); // malloc
    if (elem == NULL) {
        printf("%s: malloc failed\n", __func__);
        return NULL;
    } // Req Condition
    *elem = (Elem){
        .type = info.type,
        .anchor = info.anchor,
        .trig_func = info.trig_func,
        .gid_rect = info.gid_rect != NULL ? *info.gid_rect : ELEM_DEFAULT_GID_RECT,
    };
    if (info.string != NULL) {
        elem->string = strdup(info.string); // malloc
        if (elem->string == NULL) {
            printf("%s: failed.\n", __func__);
            return elem;
        }
    } // string
    if (info.trig_para != NULL) {
        elem->trig_para = strdup(info.trig_para);
        if (elem->trig_para == NULL) {
            printf("%s: failed.\n", __func__);
            return elem;
        }
    } // trig para
    if (info.string != NULL) {
        elem->texture = TEMPO_CreateElem_Texture(info);
        float w, h;
        SDL_GetTextureSize(elem->texture, &w, &h);
        elem->src_rect = (SDL_FRect){0, 0, w, h};
    } // texture, src_rect
    return elem;
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
Elem* TEMPO_CreateElem(const ElemInfo info) {
    Elem* elem = TEMPO_CreateElem_(info);
    if (TEMPO_CheckElem(elem) == false) {
        printf("%s: failed.\n", __func__);
        TEMPO_DeleteElem(elem);
        elem = NULL;
    }
    return elem;
}
Elem* TEMPO_CreateElemFromToml(const toml_table_t *tomlInfo) {
    if (tomlInfo == NULL) {
        printf("%s: tomlElem not exists.\n", __func__);
        return NULL;
    }

    // type
    const toml_datum_t tomlType = toml_string_in(tomlInfo, "type");
    const ElemType type = TEMPO_GetElemTypeFromString(tomlType.ok ? tomlType.u.s : NULL);

    // info
    const toml_datum_t tomlString = toml_string_in(tomlInfo, "info");
    if (!tomlString.ok) {
        printf("%s: tomlString not exists.\n", __func__);
        return NULL;
    }
    const char* string = tomlString.u.s;

    // anchor
    const toml_datum_t tomlAnchor = toml_int_in(tomlInfo, "anchor");
    const int anchor = tomlAnchor.ok ? (int)tomlAnchor.u.i : 0;

    // gid rect
    const toml_array_t* tomlGidRect = toml_array_in(tomlInfo, "guide");
    SDL_FRect gid_rect = ELEM_DEFAULT_GID_RECT;
    if (tomlGidRect != NULL) {
        loadFRectFromTomlArray(&gid_rect, tomlGidRect);
    }

    // trig func
    const toml_datum_t tomlFuncName = toml_string_in(tomlInfo, "func");
    const TrigFunc trig_func = tomlFuncName.ok ? TRIG_FindFuncFromName(tomlFuncName.u.s) : NULL;

    // trig para
    const toml_datum_t tomlPara = toml_string_in(tomlInfo, "para");
    const char* trig_para = tomlPara.ok ? tomlPara.u.s : NULL;

    //
    const ElemInfo info = {type, string, anchor, &gid_rect, trig_func, trig_para};
    Elem* elem = TEMPO_CreateElem(info);
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