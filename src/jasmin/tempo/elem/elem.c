#include "elem.h"


// ElemState
enum ElemState {
    ELEM_STATE_OUTSIDE,
    ELEM_STATE_INSIDE,
    ELEM_STATE_PRESSED,
    ELEM_STATE_RELEASE,
    NUM_ELEM_STATES
};
static char* ELEM_GetStringFromState(const ElemState state) {
    // Req Condition
    if (state >= NUM_ELEM_STATES) {
        printf("%s: state is illegal.\n", __func__);
        return NULL;
    }

    //
    static char* names[] = {"OUTSIDE", "INSIDE", "PRESSED", "RELEASED"};
    return names[state];
}


// ElemType
enum ElemType {
    ELEM_TYPE_DEFAULT,
    ELEM_TYPE_PATH,
    ELEM_TYPE_TEXT,
    NUM_ELEM_TYPES
};
static ElemStrType ELEM_GetTypeFromString(const char* string) {
    if (strcmp(string, "PATH") == 0) return ELEM_TYPE_PATH;
    if (strcmp(string, "TEXT") == 0) return ELEM_TYPE_TEXT;
    return ELEM_TYPE_DEFAULT;
}


// other
SDL_Renderer* elem_renderer;
TTF_Font* elem_font;
SDL_Color elem_color;
SDL_FRect elem_bck_rect;


// Elem
struct Elem {
    //
    int info;
    int anchor; SDL_FRect gid_rect;
    TrigFunc trig_func; char* trig_para;
    char* string;
    SDL_Texture* texture; SDL_FRect src_rect;

    //
    bool ok;

    //
    SDL_FRect dst_rect;
    ElemState state;
};


void ELEM_SetGidRect(Elem* elem, const SDL_FRect gid_rect) {
    if (elem == NULL) {
        printf("%s: elem is null.\n", __func__);
        return;
    }
    elem->src_rect = gid_rect;
}


// if
bool ELEM_IfOk(const Elem* elem) {
    return elem->ok;
}


// malloc
static SDL_Texture* ELEM_GetTextureFromString(const char* elemStr) {
    // Req Condition
    if (elemStr == NULL) {printf("%s: elemStr not exist.\n", __func__); return NULL;}
    if (elemStr[0] == '\0') {printf("%s: elemStr[%p] isn't legal.\n", __func__, elemStr); return NULL;}
    if (strlen(elemStr) <= 2) {printf("%s: elemStr[%s] isn't legal.\n", __func__, elemStr); return NULL;}

    //
    SDL_Texture* texture = NULL;
    const unsigned long elemStrLen = strlen(elemStr);
    switch (elemStr[0]) {
        // 2
        case 'f': {
            if (elemStrLen <= 2) {printf("%s: elemStr[%s] is illegal.\n", __func__, elemStr); return NULL;}
            texture = IMG_LoadTexture(elem_renderer, elemStr+2);
            break;
        }
        case 't': {
            if (elemStrLen <= 2) {printf("%s: elemStr[%s] is illegal.\n", __func__, elemStr); return NULL;}
            texture = TXT_LoadTexture(elem_renderer, elem_font, elemStr+2, elem_color);
            break;
        }
        // 3
        case 'T': {
            if (elemStrLen <= 3) {printf("%s: elemStr[%s] is illegal.\n", __func__, elemStr); return NULL;}
            texture = TXT_LoadTextureWithLines(elem_renderer, elem_font, elemStr+3, elem_color, EMPTY, *(elemStr+1));
            break;
        }
        default: {texture = NULL; break;}
    }
    if (texture == NULL) {printf("%s: failed from \"%s\".\n", __func__, elemStr);}

    return texture;
}
static void ELEM_LoadGuide(Elem* elem, const toml_array_t* tomlGuide) {
    // Req Condition
    if (elem == NULL) {printf("%s: elem not exists.\n", __func__); return;}

    // Opt Condition
    if (tomlGuide != NULL) {
        const toml_datum_t tomlGuideX = toml_double_at(tomlGuide, 0);
        const toml_datum_t tomlGuideY = toml_double_at(tomlGuide, 1);
        const toml_datum_t tomlGuideW = toml_double_at(tomlGuide, 2);
        const toml_datum_t tomlGuideH = toml_double_at(tomlGuide, 3);
        elem->gid_rect.x = (float)(tomlGuideX.ok ? tomlGuideX.u.d : 0);
        elem->gid_rect.y = (float)(tomlGuideY.ok ? tomlGuideY.u.d : 0);
        elem->gid_rect.w = (float)(tomlGuideW.ok ? tomlGuideW.u.d : 1);
        elem->gid_rect.h = (float)(tomlGuideH.ok ? tomlGuideH.u.d : 1);
    }
    else {
        elem->gid_rect.x = 0;
        elem->gid_rect.y = 0;
        elem->gid_rect.w = 1;
        elem->gid_rect.h = 1;
    }
}
static void ELEM_LoadString(Elem* elem, const toml_datum_t tomlString) {
    // Req Condition
    if (elem == NULL) {printf("%s: elem not exists.\n", __func__); return;}
    if (!tomlString.ok) {printf("%s: tomlString not exists.\n", __func__); return;}
    if (elem->string != NULL) {printf("%s: elem[%d].string not free.\n", __func__, elem->info); return;}

    //
    elem->string = strdup(tomlString.u.s);

    // Req Condition
    if (elem->string == NULL) {printf("%s: failed to malloc from \"%s\".\n", __func__, tomlString.u.s);}
}
static void ELEM_LoadPara(Elem* elem, const toml_datum_t tomlPara) {
    // Req Condition
    if (elem == NULL) {printf("%s: elem not exists.\n", __func__); return;}
    if (elem->trig_para != NULL) {printf("%s: elem.para not freed.\n", __func__); return;}

    // Opt Condition
    elem->trig_para = tomlPara.ok ? strdup(tomlPara.u.s) : NULL;
}
static void ELEM_LoadTexture(Elem* elem) {
    // Req Condition
    if (elem == NULL) {printf("%s: elem not exists.\n", __func__); return;}
    if (elem->texture != NULL) {printf("%s: elem.texture not free.\n", __func__); return;}

    //
    elem->texture = ELEM_GetTextureFromString(elem->string);
    SDL_SetTextureScaleMode(elem->texture, SDL_SCALEMODE_NEAREST);

    //
    float w, h;
    SDL_GetTextureSize(elem->texture, &w, &h);
    elem->src_rect = (SDL_FRect){0, 0, w, h};
}
static void ELEM_LoadOther(Elem* elem, const toml_table_t* tomlElem, const int tomlElemId) {
    // Req Condition
    if (elem == NULL) {printf("%s: elem not exists.\n", __func__); return;}
    if (tomlElem == NULL) {printf("%s: tomlElem not exists.\n", __func__); return;}

    // loadElemAnchorFromToml
    const toml_datum_t tomlAnchor = toml_int_in(tomlElem, "anchor");
    elem->anchor = (int)(tomlAnchor.ok ? tomlAnchor.u.i : 0);

    // loadElemFuncFromToml
    const toml_datum_t tomlFuncName = toml_string_in(tomlElem, "func");
    elem->trig_func = tomlFuncName.ok ? TRIG_FindFuncFromName(tomlFuncName.u.s) : NULL;

    // loadElemIdFromToml
    elem->info = tomlElemId;
}
static void ELEM_Load(Elem* elem, const toml_table_t* tomlElem, const int tomlElemId) {
    // Req Condition
    if (elem == NULL) {
        printf("%s: elem not exists.\n", __func__);
        return;
    }
    if (tomlElem == NULL) {
        printf("%s: tomlElem not exists.\n", __func__);
        return;
    }

    //
    ELEM_LoadOther(elem, tomlElem, tomlElemId);
    ELEM_LoadGuide(elem, toml_array_in(tomlElem, "guide"));
    ELEM_LoadPara(elem, toml_string_in(tomlElem, "para"));
    ELEM_LoadString(elem, toml_string_in(tomlElem, "string"));
    ELEM_LoadTexture(elem);
}
Elem* ELEM_Malloc(const toml_table_t* tomlElem, const int tomlElemId) {
    Elem* elem = malloc(sizeof(Elem));
    if (elem == NULL) {
        printf("malloc failed\n");
        return NULL;
    }

    //
    *elem = (Elem){0};
    ELEM_Load(elem, tomlElem, tomlElemId);
    ELEM_RenewOk(elem);
    return elem;
}


// free
void ELEM_Free(Elem* elem) {
    if (elem == NULL) {printf("%s: elem not exists.\n", __func__); return;}

    if (elem->string != NULL) {free(elem->string); elem->string = NULL;}
    if (elem->trig_para != NULL) {free(elem->trig_para); elem->trig_para = NULL;}
    if (elem->texture != NULL) {SDL_DestroyTexture(elem->texture); elem->texture = NULL;}
    free(elem);
}


// renew
void ELEM_Renew(Elem* elem) {
    // Req Condition
    if (elem->ok != true) {
        DEBUG_SendMessageR("%s: elem not ready.\n", __func__);
        return;
    }

    // ELEM_RenewDstRect
    SDL_LoadDstRectAligned(
        &elem->dst_rect,
        elem->texture,
        &elem->src_rect,
        &elem->gid_rect,
        &elem_bck_rect,
        elem->anchor
        );

    // ELEM_RenewState
    {
        const bool mouseIn = mouseInRect(&elem->dst_rect);
        const bool mouseLeftIn = mouseLeftInRect(&elem->dst_rect);
        if (elem->state == ELEM_STATE_PRESSED) {
            DEBUG_SendMessageL("elem.on: %d\n", elem->ok);
            DEBUG_SendMessageL("elem.id: %d\n", elem->info);
            DEBUG_SendMessageL("elem.state: %s\n", ELEM_GetStringFromState(elem->state));
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
}
void ELEM_RenewOk(Elem* elem) {
    if (elem == NULL) {
        printf("%s: elem not exists.\n", __func__);
        return;
    }
    elem->ok = false;
    if (elem->info < 0) {
        printf("%s: elem.id is illegal.\n", __func__);
        return;
    }
    if (elem->string == NULL) {
        printf("%s: elem[%d].string not exists.\n", __func__, elem->info);
        return;
    }
    if (elem->texture == NULL) {
        printf("%s: elem[%d].texture not exists.\n", __func__, elem->info);
        return;
    }
    elem->ok = true;
}


// draw
void ELEM_Draw(const Elem* elem) {
    // Req Condition
    if (elem->ok != true) {
        DEBUG_SendMessageR("%s: elem not ready.\n", __func__);
        return;
    }
    if (elem_renderer == NULL) {printf("%s: menu.renderer is NULL.\n", __func__); return;}

    //
    switch (elem->state) {
        case ELEM_STATE_PRESSED:
        case ELEM_STATE_RELEASE: {
            DEBUG_FillRect(&elem->dst_rect);
            break;
        }
        case ELEM_STATE_INSIDE: {
            DEBUG_DrawRect(&elem->dst_rect);
            break;
        }
        case ELEM_STATE_OUTSIDE:
        default: break;
    }
    SDL_RenderTexture(elem_renderer, elem->texture, &elem->src_rect, &elem->dst_rect);
}