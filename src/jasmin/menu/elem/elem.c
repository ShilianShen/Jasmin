#include "elem.h"


SDL_Renderer* elem_renderer;
TTF_Font* elem_font;
SDL_Color elem_color;
SDL_FRect elem_bck_rect;


void ELEM_Init(Elem* elem) {
    // Req Condition
    if (elem == NULL) {printf("%s: elem not exists.\n", __func__); return;}

    //
    *elem = (Elem){0};
}
void ELEM_Deinit(const Elem* elem) {
    // Opt Condition
    if (elem != NULL) {}
}


void ELEM_TurnOn(Elem* elem) {
    // Req Condition
    if (elem == NULL) {
        printf("%s: elem not exists.\n", __func__);
        return;
    }

    //
    elem->on = true;
}
void ELEM_TurnOff(Elem* elem) {
    // Req Condition
    if (elem == NULL) {
        printf("%s: elem not exists.\n", __func__);
        return;
    }

    //
    elem->on = false;
}
bool ELEM_IfReady(const Elem* elem) {
    // Req Condition
    if (elem == NULL) {
        printf("%s: elem not exists.\n", __func__);
        return false;
    }
    if (elem->on == false) {
        printf("%s: elem is off.\n", __func__);
        return false;
    }
    if (elem->id < 0) {
        printf("%s: elem.id is illegal.\n", __func__);
        return false;
    }
    if (elem->string == NULL) {
        printf("%s: elem[%d].string not exists.\n", __func__, elem->id);
        return false;
    }
    if (elem->texture == NULL) {
        printf("%s: elem[%d].texture not exists.\n", __func__, elem->id);
        return false;
    }

    //
    return true;
}
char* ELEM_GetStateName(const ElemState state) {
    // Req Condition
    if (state >= NUM_ELEM_STATES) {
        printf("%s: state is illegal.\n", __func__);
        return NULL;
    }

    //
    static char* names[] = {"OUTSIDE", "INSIDE", "PRESSED", "RELEASED"};
    return names[state];
}


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
        elem->guide.x = (float)(tomlGuideX.ok ? tomlGuideX.u.d : 0);
        elem->guide.y = (float)(tomlGuideY.ok ? tomlGuideY.u.d : 0);
        elem->guide.w = (float)(tomlGuideW.ok ? tomlGuideW.u.d : 1);
        elem->guide.h = (float)(tomlGuideH.ok ? tomlGuideH.u.d : 1);
    }
    else {
        elem->guide.x = 0;
        elem->guide.y = 0;
        elem->guide.w = 1;
        elem->guide.h = 1;
    }
}
static void ELEM_LoadString(Elem* elem, const toml_datum_t tomlString) {
    // Req Condition
    if (elem == NULL) {printf("%s: elem not exists.\n", __func__); return;}
    if (!tomlString.ok) {printf("%s: tomlString not exists.\n", __func__); return;}
    if (elem->string != NULL) {printf("%s: elem[%d].string not free.\n", __func__, elem->id); return;}

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
    elem->id = tomlElemId;
}
void ELEM_Load(Elem* elem, const toml_table_t* tomlElem, const int tomlElemId) {
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
    ELEM_TurnOn(elem);
}


void ELEM_Unload(Elem* elem) {
    // Req Condition
    if (elem == NULL) {printf("%s: elem not exists.\n", __func__); return;}

    //
    if (elem->string != NULL) {free(elem->string); elem->string = NULL;}
    if (elem->trig_para != NULL) {free(elem->trig_para); elem->trig_para = NULL;}
    if (elem->texture != NULL) {SDL_DestroyTexture(elem->texture); elem->texture = NULL;}
    ELEM_TurnOff(elem);
}


static void ELEM_RenewDstRect(Elem* elem) {
    // Req Condition
    if (ELEM_IfReady(elem) == false) {
        DEBUG_SendMessageR("%s: elem not ready.\n", __func__);
        return;
    }

    //
    SDL_LoadDstRectAligned(&elem->dst_rect, elem->texture, &elem->src_rect, &elem->guide, &elem_bck_rect, elem->anchor);
}
static void ELEM_RenewState(Elem* elem) {
    // Req Condition
    if (!ELEM_IfReady(elem)) {
        DEBUG_SendMessageR("%s: elem not ready.\n", __func__);
        return;
    }

    //
    const bool mouseIn = mouseInRect(&elem->dst_rect);
    const bool mouseLeftIn = mouseLeftInRect(&elem->dst_rect);
    if (elem->state == PRESSED) {
        DEBUG_SendMessageL("elem.on: %d\n", elem->on);
        DEBUG_SendMessageL("elem.id: %d\n", elem->id);
        DEBUG_SendMessageL("elem.state: %s\n", ELEM_GetStateName(elem->state));
        if (elem->trig_func != NULL) {
            DEBUG_SendMessageL("elem.trig: %s, %s\n", TRIG_FindNameFromFunc(elem->trig_func), elem->trig_para);
        }
    }
    if (elem->state == PRESSED && mouseIn == true && mouseLeftIn == false) {
        elem->state = RELEASE;
    }
    else {
        if (mouseIn == true) {
            elem->state = mouseLeftIn ? PRESSED : INSIDE;
        }
        else {
            elem->state = OUTSIDE;
        }
    }
    if (elem->state == RELEASE && elem->trig_func != NULL) {
        elem->trig_func(elem->trig_para);
    }
}
void ELEM_Renew(Elem* elem) {
    // Req Condition
    if (ELEM_IfReady(elem) == false) {
        DEBUG_SendMessageR("%s: elem not ready.\n", __func__);
        return;
    }

    //
    ELEM_RenewDstRect(elem);
    ELEM_RenewState(elem);
}


void ELEM_Draw(const Elem* elem) {
    // Req Condition
    if (ELEM_IfReady(elem) == false) {
        DEBUG_SendMessageR("%s: elem not ready.\n", __func__);
        return;
    }
    if (elem_renderer == NULL) {printf("%s: menu.renderer is NULL.\n", __func__); return;}

    //
    switch (elem->state) {
        case PRESSED:
        case RELEASE: {
            DEBUG_FillRect(&elem->dst_rect);
            break;
        }
        case INSIDE: {
            DEBUG_DrawRect(&elem->dst_rect);
            break;
        }
        case OUTSIDE:
        default: break;
    }
    SDL_RenderTexture(elem_renderer, elem->texture, &elem->src_rect, &elem->dst_rect);
}