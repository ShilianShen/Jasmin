#include "elem.h"


const SDL_FRect ELEM_DEFAULT_GID_RECT = {0, 0, 1, 1};


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
    ElemInfo info;
    SDL_FRect gid_rect;

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


// CREATE ==============================================================================================================
static void TEMPO_CreateElem_CheckOk(Elem* elem) {
    elem->ok = false;
    if (elem->info.string == NULL) {
        printf("%s: elem.string not exists.\n", __func__);
        return;
    }
    if (elem->texture == NULL) {
        printf("%s: elem[%s].texture not exists.\n", __func__, elem->info.string);
        return;
    }
    elem->ok = true;
}
Elem* TEMPO_CreateElem(const ElemInfo info)
{
    if (info.string == NULL) {
        printf("%s: info is null.\n", __func__);
        return NULL;
    } // Req Condition


    Elem* elem = malloc(sizeof(Elem)); // malloc
    if (elem == NULL) {
        printf("%s: malloc failed\n", __func__);
        return NULL;
    } // Req Condition


    *elem = (Elem){0};


    elem->info.type = info.type;
    elem->info.string = strdup(info.string); // malloc
    if (elem->info.string == NULL) {
        printf("%s: failed to malloc from \"%s\".\n", __func__, info.string);
        return elem;
    } // Req Condition


    elem->info.anchor = info.anchor;
    if (info.gid_rect != NULL) {
        elem->gid_rect = *info.gid_rect;
    }
    else {
        elem->gid_rect = ELEM_DEFAULT_GID_RECT;
    }


    elem->info.trig_func = info.trig_func;
    if (info.trig_para != NULL) {
        elem->info.trig_para = strdup(info.trig_para); // malloc
        if (elem->info.trig_para == NULL) {
            printf("%s: failed to malloc from \"%s\".\n", __func__, info.trig_para);
            return elem;
        } // Req Condition
    } // Opt Condition


    switch (info.type) {
        case ELEM_TYPE_FILE: {
            elem->texture = IMG_LoadTexture(tempoRenderer, elem->info.string); // malloc
            break;
        }
        case ELEM_TYPE_TEXT: {
            elem->texture = TXT_LoadTextureWithLines(tempoRenderer, tempoTextFont, elem->info.string, (SDL_Color){255, 255, 255, 255}, EMPTY, 'C'); // malloc
            break;
        }
        default: {
            elem->texture = NULL;
            break;
        }
    }
    if (elem->texture == NULL) {
        printf("%s: failed from \"%s\".\n", __func__, elem->info.string);
        return elem;
    } // Req Condition
    SDL_SetTextureScaleMode(elem->texture, SDL_SCALEMODE_NEAREST);


    float w, h;
    SDL_GetTextureSize(elem->texture, &w, &h);
    elem->src_rect = (SDL_FRect){0, 0, w, h};


    TEMPO_CreateElem_CheckOk(elem);
    return elem;
}
Elem* TEMPO_CreateElemFromToml(const toml_table_t* tomlElem) {
    if (tomlElem == NULL) {
        printf("%s: tomlElem not exists.\n", __func__);
        return NULL;
    }

    // type
    const toml_datum_t tomlType = toml_string_in(tomlElem, "type");
    const ElemType type = TEMPO_GetElemTypeFromString(tomlType.ok ? tomlType.u.s : NULL);

    // info
    const toml_datum_t tomlInfo = toml_string_in(tomlElem, "info");
    if (!tomlInfo.ok) {
        printf("%s: tomlString not exists.\n", __func__);
        return NULL;
    }
    const char* string = tomlInfo.u.s;

    // anchor
    const toml_datum_t tomlAnchor = toml_int_in(tomlElem, "anchor");
    const int anchor = tomlAnchor.ok ? (int)tomlAnchor.u.i : 0;

    // gid rect
    const toml_array_t* tomlGidRect = toml_array_in(tomlElem, "guide");
    SDL_FRect gid_rect = ELEM_DEFAULT_GID_RECT;
    loadFRectFromTomlArray(&gid_rect, tomlGidRect);

    // trig func
    const toml_datum_t tomlFuncName = toml_string_in(tomlElem, "func");
    const TrigFunc trig_func = tomlFuncName.ok ? TRIG_FindFuncFromName(tomlFuncName.u.s) : NULL;

    // trig para
    const toml_datum_t tomlPara = toml_string_in(tomlElem, "para");
    const char* trig_para = tomlPara.ok ? tomlPara.u.s : NULL;

    //
    const ElemInfo info = {type, string, anchor, &gid_rect, trig_func, trig_para};
    Elem* elem = TEMPO_CreateElem(info);
    return elem;
}


// DESTROY =============================================================================================================
void TEMPO_DestroyElem(Elem* elem) {
    if (elem != NULL) {
        if (elem->info.string != NULL) {
            free((void*)elem->info.string); // free
            elem->info.string = NULL;
        }
        if (elem->info.trig_para != NULL) {
            free((void*)elem->info.trig_para); // free
            elem->info.trig_para = NULL;
        }
        if (elem->texture != NULL) {
            SDL_DestroyTexture(elem->texture); // free
            elem->texture = NULL;
        }
        free(elem); // free
    }
}


// RENEW ===============================================================================================================
static void TEMPO_RenewElem_DstRect(Elem* elem) {
    SDL_LoadDstRectAligned(
        &elem->dst_rect,
        elem->texture,
        &elem->src_rect,
        &elem->gid_rect,
        &tempoBckRect,
        elem->info.anchor
        );
}
static void TEMPO_RenewElem_State(Elem* elem) {
    if (elem->visible == false) {
        return;
    }
    const bool mouseIn = mouseInRect(&elem->dst_rect);
    const bool mouseLeftIn = mouseLeftInRect(&elem->dst_rect);
    if (elem->state == ELEM_STATE_PRESSED) {
        DEBUG_SendMessageL("elem: %s\n", elem->info.string);
        DEBUG_SendMessageL("elem.on: %d\n", elem->ok);
        DEBUG_SendMessageL("elem.state: %s\n", TEMPO_GetStringFromElemState(elem->state));
        if (elem->info.trig_func != NULL) {
            DEBUG_SendMessageL("elem.trig: %s, %s\n", TRIG_FindNameFromFunc(elem->info.trig_func), elem->info.trig_para);
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
    if (elem->state == ELEM_STATE_RELEASE && elem->info.trig_func != NULL) {
        elem->info.trig_func(elem->info.trig_para);
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