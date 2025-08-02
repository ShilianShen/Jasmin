#include "elem.h"


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
    [ELEM_TYPE_DEFAULT] = "NULL",
    [ELEM_TYPE_FILE] = "FILE",
    [ELEM_TYPE_TEXT] = "TEXT"
};
static ElemType TEMPO_GetElemTypeFromString(const char* string) {
    for (int i = 0; i < ELEM_NUM_TYPES; i++) {
        if (strcmp(string, ELEM_TYPE_STRINGS[i]) == 0) {
            return i;
        }
    }
    return ELEM_TYPE_DEFAULT;
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
    bool ok; // ok为true意味着可以正常renew和draw

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
bool TEMPO_GetElemOk(const Elem* elem) {
    if (elem == NULL) {
        printf("%s: elem is null.\n", __func__);
        return false;
    }
    return elem->ok;
}


// ?
static void TEMPO_CreateElem_CheckOk(Elem* elem) {
    elem->ok = false;
    if (elem->string == NULL) {
        printf("%s: elem.string not exists.\n", __func__);
        return;
    }
    if (elem->texture == NULL) {
        printf("%s: elem[%s].texture not exists.\n", __func__, elem->string);
        return;
    }
    elem->ok = true;
}


// LOAD & UNLOAD =======================================================================================================
static bool TEMPO_LoadElem_Texture(Elem* elem, const ElemInfo info) {
    switch (info.type) {
        case ELEM_TYPE_FILE: {
            elem->texture = IMG_LoadTexture(tempoRenderer, elem->string); // malloc
            break;
        }
        case ELEM_TYPE_TEXT: {
            elem->texture = TXT_LoadTextureWithLines(
                tempoRenderer,
                tempoTextFont,
                elem->string,
                (SDL_Color){255, 255, 255, 255},
                EMPTY,
                'C'
                ); // malloc
            break;
        }
        default: {
            elem->texture = NULL;
            break;
        }
    }
    if (elem->texture == NULL) {
        printf("%s: failed from \"%s\".\n", __func__, elem->string);
        return false;
    } // Req Condition
    SDL_SetTextureScaleMode(elem->texture, SDL_SCALEMODE_NEAREST);
    return true;
}
static bool TEMPO_LoadElem(Elem* elem, const ElemInfo info) {
    // others
    elem->type = info.type;
    elem->anchor = info.anchor;
    elem->gid_rect = info.gid_rect != NULL ? *info.gid_rect : ELEM_DEFAULT_GID_RECT;
    elem->trig_func = info.trig_func;

    // string
    if (info.string == NULL) {
        printf("%s: info is null.\n", __func__);
        return false;
    } // Req Condition
    elem->string = strdup(info.string); // malloc
    if (elem->string == NULL) {
        printf("%s: failed to malloc from \"%s\".\n", __func__, info.string);
        return false;
    } // Req Condition

    // trig_para
    if (info.trig_para != NULL) {
        elem->trig_para = strdup(info.trig_para); // malloc
        if (elem->trig_para == NULL) {
            printf("%s: failed to malloc from \"%s\".\n", __func__, info.trig_para);
            return false;
        } // Req Condition
    } // Opt Condition

    // texture
    if (TEMPO_LoadElem_Texture(elem, info) == false) {
        printf("%s: failed from \"%s\".\n", __func__, elem->string);
        return false;
    } // Req Condition

    // scr_rect
    float w, h;
    SDL_GetTextureSize(elem->texture, &w, &h);
    elem->src_rect = (SDL_FRect){0, 0, w, h};

    // ok
    TEMPO_CreateElem_CheckOk(elem);
    return true;
}
static void TEMPO_UnloadElem(Elem* elem) {
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


// CREATE & DESTROY ====================================================================================================
Elem* TEMPO_CreateElem(const ElemInfo info) {
    Elem* elem = malloc(sizeof(Elem)); // malloc
    if (elem == NULL) {
        printf("%s: malloc failed\n", __func__);
        return NULL;
    } // Req Condition
    *elem = (Elem){0};
    if (TEMPO_LoadElem(elem, info) == false) {
        TEMPO_DestroyElem(elem);
        elem = NULL;
    }
    return elem;
} // 创建Elem(动态分配), 如果失败返回NULL.
void TEMPO_DestroyElem(Elem* elem) {
    TEMPO_UnloadElem(elem);
    free(elem); // free
    elem = NULL;
}
Elem* TEMPO_CreateElemFromToml(const toml_table_t* tomlInfo) {
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
    loadFRectFromTomlArray(&gid_rect, tomlGidRect);

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
static void TEMPO_RenewElem_DstRect(Elem* elem) {
    SDL_LoadDstRectAligned(
        &elem->dst_rect,
        elem->texture,
        &elem->src_rect,
        &elem->gid_rect,
        &tempoBckRect,
        elem->anchor
        );
}
static void TEMPO_RenewElem_State(Elem* elem) {
    if (elem->visible == false) {
        return;
    }
    const bool mouseIn = mouseInRect(&elem->dst_rect);
    const bool mouseLeftIn = mouseLeftInRect(&elem->dst_rect);
    if (elem->state == ELEM_STATE_PRESSED) {
        DEBUG_SendMessageL("elem: %s\n", elem->string);
        DEBUG_SendMessageL("elem.on: %d\n", elem->ok);
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
    if (elem->ok != true) {
        DEBUG_SendMessageR("%s: elem not ready.\n", __func__);
        return;
    }
    TEMPO_RenewElem_State(elem);
    elem->visible = false;
    TEMPO_RenewElem_DstRect(elem);
}


// DRAW ================================================================================================================
static bool TEMPO_DrawElem_(const Elem* elem) {
    // Req Condition

    //
    switch (elem->state) {
        case ELEM_STATE_PRESSED: {
            DEBUG_FillRect(&elem->dst_rect);
            SDL_RenderTexture(tempoRenderer, elem->texture, &elem->src_rect, &elem->dst_rect);
            DEBUG_DrawRect(&elem->dst_rect);
            break;
        }
        case ELEM_STATE_RELEASE:
        case ELEM_STATE_INSIDE: {
            SDL_RenderTexture(tempoRenderer, elem->texture, &elem->src_rect, &elem->dst_rect);
            DEBUG_DrawRect(&elem->dst_rect);
            break;
        }
        case ELEM_STATE_OUTSIDE: {
            SDL_RenderTexture(tempoRenderer, elem->texture, &elem->src_rect, &elem->dst_rect);
            break;
        }
        default: break;
    }
    return true;
}
void TEMPO_DrawElem(Elem *elem) {
    // Req Condition
    if (TEMPO_GetElemOk(elem) != true) {
        DEBUG_SendMessageR("%s: elem not ready.\n", __func__);
        return;
    }
    elem->visible = false;
    if (tempoRenderer == NULL) {
        printf("%s: menu.renderer is NULL.\n", __func__);
        return;
    }
    elem->visible = TEMPO_DrawElem_(elem);
}