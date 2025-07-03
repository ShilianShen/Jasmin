#include "func.h"


static SDL_Texture* getTextureFromElemString(const ElemStr elemStr) {
    // Req Condition
    if (elemStr == NULL) {printf("%s: elemStr not exist.\n", __func__); return NULL;}
    if (elemStr[0] == '\0') {printf("%s: elemStr[%p] isn't legal.\n", __func__, elemStr); return NULL;}
    if (strlen(elemStr) <= 2) {printf("%s: elemStr[%s] isn't legal.\n", __func__, elemStr); return NULL;}

    //
    SDL_Texture* texture = NULL;
    const int elemStrLen = strlen(elemStr);
    switch (elemStr[0]) {
        // 2
        case 'f': {
            if (elemStrLen <= 2) {printf("%s: elemStr[%s] isn't legal.\n", __func__, elemStr); return NULL;}
            texture = IMG_LoadTexture(menu.renderer, elemStr+2);
            break;
        }
        case 't': {
            if (elemStrLen <= 2) {printf("%s: elemStr[%s] isn't legal.\n", __func__, elemStr); return NULL;}
            texture = TXT_LoadTexture(menu.renderer, menu.theme.font, elemStr+2, menu.theme.color);
            break;
        }
        // 3
        case 'T': {
            if (elemStrLen <= 3) {printf("%s: elemStr[%s] isn't legal.\n", __func__, elemStr); return NULL;}
            texture = TXT_LoadTextureWithLines(menu.renderer, menu.theme.font, elemStr+3, menu.theme.color, EMPTY, *(elemStr+1));
            break;
        }
        default: {texture = NULL; break;}
    }
    if (texture == NULL) {printf("%s: failed from \"%s\".\n", __func__, elemStr);}

    return texture;
}


static void MENU_LoadElemGuide(Elem* elem, const toml_array_t* tomlGuide) {
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
static void MENU_LoadElemString(Elem* elem, const toml_datum_t tomlString) {
    // Req Condition
    if (elem == NULL) {printf("%s: elem not exists.\n", __func__); return;}
    if (!tomlString.ok) {printf("%s: tomlString not exists.\n", __func__); return;}
    if (elem->string != NULL) {printf("%s: elem[%d].string not free.\n", __func__, elem->id); return;}

    //
    elem->string = strdup(tomlString.u.s);

    // Req Condition
    if (elem->string == NULL) {printf("%s: failed to malloc from \"%s\".\n", __func__, tomlString.u.s);}
}
static void MENU_LoadElemPara(Elem* elem, const toml_datum_t tomlPara) {
    // Req Condition
    if (elem == NULL) {printf("%s: elem not exists.\n", __func__); return;}
    if (elem->trig_para != NULL) {printf("%s: elem.para not freed.\n", __func__); return;}

    // Opt Condition
    elem->trig_para = tomlPara.ok ? strdup(tomlPara.u.s) : NULL;
}
static void MENU_LoadElemTexture(Elem* elem) {
    // Req Condition
    if (elem == NULL) {printf("%s: elem not exists.\n", __func__); return;}
    if (elem->texture != NULL) {printf("%s: elem.texture not free.\n", __func__); return;}

    //
    elem->texture = getTextureFromElemString(elem->string);
    SDL_SetTextureScaleMode(elem->texture, SDL_SCALEMODE_NEAREST);

    //
    float w, h;
    SDL_GetTextureSize(elem->texture, &w, &h);
    elem->src_rect = (SDL_FRect){0, 0, w, h};
}
static void MENU_LoadElemOther(Elem* elem, const toml_table_t* tomlElem, const int tomlElemId) {
    // Req Condition
    if (elem == NULL) {printf("%s: elem not exists.\n", __func__); return;}
    if (tomlElem == NULL) {printf("%s: tomlElem not exists.\n", __func__); return;}

    // loadElemAnchorFromToml
    const toml_datum_t tomlAnchor = toml_int_in(tomlElem, "anchor");
    elem->anchor = (Anchor)(tomlAnchor.ok ? tomlAnchor.u.i : 0);

    // loadElemFuncFromToml
    const toml_datum_t tomlFuncName = toml_string_in(tomlElem, "func");
    elem->trig_func = tomlFuncName.ok ? TRIG_FindFuncFromName(tomlFuncName.u.s) : NULL;

    // loadElemIdFromToml
    elem->id = tomlElemId;
}
static void MENU_LoadElem(Elem* elem, const toml_table_t* tomlElem, const int tomlElemId) {
    // Req Condition
    if (elem == NULL) {printf("%s: elem not exists.\n", __func__); return;}
    if (tomlElem == NULL) {printf("%s: tomlElem not exists.\n", __func__); return;}

    //
    MENU_LoadElemOther(elem, tomlElem, tomlElemId);
    MENU_LoadElemGuide(elem, toml_array_in(tomlElem, "guide"));
    MENU_LoadElemPara(elem, toml_string_in(tomlElem, "para"));
    MENU_LoadElemString(elem, toml_string_in(tomlElem, "string"));
    MENU_LoadElemTexture(elem);
}


static toml_table_t* getToml(const char* tomlPath) {
    // Req Condition
    FILE* file = fopen(tomlPath, "r"); // malloc
    if (file == NULL) {printf("%s: failed to open \"%s\".\n", __func__, tomlPath); return NULL;}

    //
    toml_table_t* toml = toml_parse_file(file, NULL, 0); // malloc
    fclose(file); // end malloc

    // Req Condition
    if (toml == NULL) {printf("%s: failed to read \"%s\".\n", __func__, tomlPath); return NULL;}

    return toml;
}
static void MENU_LoadPageName(Page* page, const char* name) {
    // Req Condition
    if (page == NULL) {printf("%s: page not exists.\n", __func__); return;}
    if (name == NULL) {printf("%s: name not exists.\n", __func__); return;}

    //
    page->name = strdup(name);

    // Req Condition
    if (page->name == NULL) {printf("%s: failed.\n", __func__);}
}
static void MENU_LoadPageElems(Page* page, const toml_array_t* tomlElems) {
    // Req Condition
    if (page == NULL) {printf("%s: page not exists.\n", __func__); return;}
    if (tomlElems == NULL) {printf("%s: tomlElems not exists.\n", __func__); return;}

    // Req Condition
    const int tomlElemsVolume = toml_array_nelem(tomlElems);
    if (tomlElemsVolume >= PAGE_MAX_VOLUME) {
        printf("%s: tomlPage[%s].tomlElems overflows.\n", __func__, page->name);
        return;
    }

    //
    for (PageVolume i = 0; i < tomlElemsVolume; i++) {
        const toml_table_t* tomlElem = toml_table_at(tomlElems, i);
        // Opt Condition
        if (tomlElem != NULL) {
            MENU_LoadElem(&page->elems[i], tomlElem, i+1);
        }
    }
}
static void MENU_LoadPage(Page* page, const char* name, const toml_table_t* tomlPage) {
    // Req Condition
    if (page == NULL) {printf("%s: page not exists.\n", __func__); return;}
    if (name == NULL) {printf("%s: name not exists.\n", __func__); return;}
    if (tomlPage == NULL) {printf("%s: tomlPage not exists.\n", __func__); return;}

    //
    MENU_LoadPageName(page, name);
    MENU_LoadPageElems(page, toml_array_in(tomlPage, "elems"));
}


static void MENU_LoadMenuThemeFont(const toml_datum_t tomlFontPath, const toml_datum_t tomlFontSize) {
    // Req Condition
    if (menu.theme.font != NULL) {printf("%s: theme.font not free.\n", __func__); return;}
    if (!tomlFontPath.ok) {printf("%s: tomlFontPath not exists.\n", __func__); return;}
    if (!tomlFontSize.ok) {printf("%s: tomlFontSize not exists.\n", __func__); return;}

    //
    menu.theme.font = TTF_OpenFont(tomlFontPath.u.s, tomlFontSize.u.i);
    if (menu.theme.font == NULL) {printf("%s: failed from %s.\n", __func__, tomlFontPath.u.s);}
}
static void MENU_LoadMenuThemeColor(const toml_array_t* tomlColor) {
    // Req Condition
    if (tomlColor == NULL) {printf("%s: tomlColor not exists.\n", __func__); return;}

    //
    int color[4] = {};
    for (int i = 0; i < 4; i++) {
        const toml_datum_t c = toml_int_at(tomlColor, i);
        color[i] = c.ok ? c.u.i : 255;
    }
    menu.theme.color.r = color[0];
    menu.theme.color.g = color[1];
    menu.theme.color.b = color[2];
    menu.theme.color.a = color[3];
}
static void MENU_LoadMenuTheme(const char* tomlPath) {
    // Req Condition
    toml_table_t* tomlMenuTheme = getToml(tomlPath);
    if (tomlMenuTheme == NULL) {printf("%s: failed from \"%s\".\n", __func__, tomlPath); return;}

    //
    MENU_LoadMenuThemeFont(
        toml_string_in(tomlMenuTheme, "font_path"),
        toml_int_in(tomlMenuTheme, "font_size"));
    MENU_LoadMenuThemeColor(toml_array_in(tomlMenuTheme, "font_color"));

    //
    toml_free(tomlMenuTheme);
}
static void MENU_LoadMenuPages(const char* tomlPath) {
    // Req Condition
    toml_table_t* tomlMenu = getToml(tomlPath);
    if (tomlMenu == NULL) {printf("%s: failed from \"%s\".\n", __func__, tomlPath); return;}

    // getPageName
    PageId pageId = 1;
    for (int i = 0; toml_key_in(tomlMenu, i) != NULL; i++) {
        const char* pageName = toml_key_in(tomlMenu, i);
        const toml_table_t* tomlPage = toml_table_in(tomlMenu, pageName);
        if (tomlPage == NULL) {
            printf("%s: failed to get \"%s\" from tomlMenu[%p].\n", __func__, pageName, tomlMenu);
        }
        else if (strcmp(pageName, MENU_ROOT_NAME) == 0) {MENU_LoadPage(menu.pageRoot, pageName, tomlPage);}
        else if (strcmp(pageName, MENU_EDGE_NAME) == 0) {MENU_LoadPage(menu.pageEdge, pageName, tomlPage);}
        else {
            if (menu.pages[pageId] == NULL) {
                menu.pages[pageId] = malloc(sizeof(Page));
            }
            MENU_LoadPage(menu.pages[pageId], pageName, tomlPage);
            pageId++;
        }
    }

    //
    toml_free(tomlMenu); // end malloc
}
static void MENU_LoadMenu(const char* menuPagesPath, const char* menuThemePath) {
    // Req Condition
    if (menu.renderer == NULL) {printf("%s: renderer not exists.\n", __func__); return;}

    //
    MENU_LoadMenuTheme(menuThemePath);
    MENU_LoadMenuPages(menuPagesPath);
}


void MENU_Load(const char* menuPagesPath, const char* menuThemePath) {
    MENU_LoadMenu(menuPagesPath, menuThemePath);
}