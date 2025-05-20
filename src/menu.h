#ifndef MENU_H
#define MENU_H


#pragma region TRIG ====================================================================================================


typedef char* TrigPara;
typedef char* TrigName;
typedef void (*TrigFunc)(TrigPara);
typedef struct {const TrigName name; const TrigFunc func;} Trig;


#pragma endregion TRIG  ================================================================================================
#pragma region ELEM ====================================================================================================


typedef uint8_t Anchor;
typedef uint8_t ElemId;
typedef char ElemStr;
typedef enum {OUTSIDE, INSIDE, PRESSED, RELEASE, NUM_ELEM_STATES} ElemState;


typedef struct {
    ElemId id;
    bool on;

    // file
    Anchor anchor;
    SDL_FRect guide;
    TrigFunc func;
    TrigPara para; // malloc
    ElemStr* string; // malloc

    // graphic
    SDL_Texture* texture; // malloc

    // renewable
    SDL_FRect src_rect, dst_rect;
    ElemState state;

} Elem;


#pragma endregion ELEM =================================================================================================
#pragma region PAGE ====================================================================================================


typedef char PageName;
typedef int PageVolume;
const PageVolume PAGE_MAX_VOLUME = 32;


typedef struct {
    PageName* name;
    Elem elems[PAGE_MAX_VOLUME];
} Page;


#pragma endregion PAGE =================================================================================================
#pragma region MENU ====================================================================================================


typedef int PageId;
typedef int PathId;
const PageId MENU_PAGE_VOLUME = 16;
const PathId MENU_PATH_VOLUME = 6;


struct {
    SDL_Renderer* renderer;
    SDL_FRect bck_rect;

    PageId path[MENU_PATH_VOLUME];
    char pathString[128];

    Page *pageRoot, *pageEdge, *pageNow, *pages[MENU_PAGE_VOLUME];

    struct {TTF_Font* font; SDL_Color color;} theme;
} menu;


#pragma endregion MENU =================================================================================================
#pragma region TRIG_FUNC ===============================================================================================


void trigFuncPass(const TrigPara para) {}
void trigFuncForward(const TrigPara pageName) {
    ma_engine_play_sound(&maEngine, "../sound effects/ring01.wav", NULL);

    // getPageId
    PageId pageId = 0;
    for (PageId i = 0; i < MENU_PAGE_VOLUME; i++) {
        if (menu.pages[i] == NULL) {continue;}
        if (strcmp(menu.pages[i]->name, pageName) == 0) {pageId = i;}
    }
    if (pageId == 0) {printf("%s: \"%s\" not exists.\n", __func__, (char*)pageName); return;}

    // forward
    for (PathId i = 0; i < MENU_PATH_VOLUME; i++) {
        if (menu.path[i] == 0) {
            menu.path[i] = pageId;
            break;
        }
    }
}
void trigFuncBackward(const TrigPara para) {
    ma_engine_play_sound(&maEngine, "../sound effects/ring08.wav", NULL);

    for (PathId i = MENU_PATH_VOLUME - 1; i >= 0; i--) {
        if (menu.path[i] != 0) {
            menu.path[i] = 0;
            break;
        }
    }
}
void trigFuncClear(const TrigPara para) {
    ma_engine_play_sound(&maEngine, "../sound effects/ring08.wav", NULL);
    for (PathId i = 0; i < MENU_PATH_VOLUME; i++) {menu.path[i] = 0;}
}
const Trig triggers[] = {
    {"pass", trigFuncPass},
    {"forward", trigFuncForward},
    {"backward", trigFuncBackward},
    {"clear", trigFuncClear},
    {NULL, NULL}
};
TrigFunc getTrigFuncFromName(const TrigName name) {
    // N-Condition
    if (name == NULL) {printf("%s: name not exists.\n", __func__); return NULL;}

    //
    for (int i = 0; triggers[i].name != NULL; i++) {
        if (strcmp(triggers[i].name, name) == 0) {
            return triggers[i].func;
        }
    }
    return NULL;
}
TrigName getTrigNameFromFunc(const TrigFunc func) {
    // N-Condition
    if (func == NULL) {printf("%s: func not exists.\n", __func__); return NULL;}

    //
    for (int i = 0; triggers[i].func != NULL; i++) {
        if (triggers[i].func == func) {
            return triggers[i].name;
        }
    }
    return NULL;
}


#pragma endregion TRIG_FUNC ============================================================================================
#pragma region ELEM_FUNC ===============================================================================================


// other
SDL_Texture* getTextureFromElemString(const ElemStr* elemStr) {
    // Req Condition
    if (elemStr == NULL) {printf("%s: elemStr not exist.\n", __func__); return NULL;}
    if (elemStr[0] == '\0') {printf("%s: elemStr[%p] isn't legal.\n", __func__, elemStr); return NULL;}
    if (strlen(elemStr) <= 2) {printf("%s: elemStr[%s] isn't legal.\n", __func__, elemStr); return NULL;}

    //
    SDL_Texture* texture = NULL;
    const int elemStrLen = strlen(elemStr);
    switch (elemStr[0]) {
        // 0
        case 'p': {
            texture = TXT_LoadTexture(menu.renderer, menu.theme.font, menu.pathString, menu.theme.color);
            break;
        }
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
            texture = TXT_LoadTextureWithLines(menu.renderer, menu.theme.font, elemStr+3, menu.theme.color, *(elemStr+1));
            break;
        }
        default: {texture = NULL; break;}
    }
    if (texture == NULL) {printf("%s: failed from \"%s\".\n", __func__, elemStr);}

    return texture;
}
void turnElemOn(Elem* elem) {
    // Req Condition
    if (elem == NULL) {printf("%s: elem not exists.\n", __func__); return;}

    //
    elem->on = true;
}
void turnElemOff(Elem* elem) {
    // Req Condition
    if (elem == NULL) {printf("%s: elem not exists.\n", __func__); return;}

    //
    elem->on = false;
}


// load & kill
void killElem(Elem* elem) {
    // Req Condition
    if (elem == NULL) {printf("%s: elem not exists.\n", __func__); return;}

    //
    if (elem->string != NULL) {free(elem->string); elem->string = NULL;}
    if (elem->para != NULL) {free(elem->para); elem->para = NULL;}
    if (elem->texture != NULL) {SDL_DestroyTexture(elem->texture); elem->texture = NULL;}

    // ?
    turnElemOff(elem);
}

void loadElemGuide(Elem* elem, const toml_array_t* tomlGuide) {
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
void loadElemString(Elem* elem, const toml_datum_t tomlString) {
    // Req Condition
    if (elem == NULL) {printf("%s: elem not exists.\n", __func__); return;}
    if (!tomlString.ok) {printf("%s: tomlString not exists.\n", __func__); return;}
    if (elem->string != NULL) {printf("%s: elem[%d].string not free.\n", __func__, elem->id); return;}

    //
    elem->string = strdup(tomlString.u.s);

    // Req Condition
    if (elem->string == NULL) {printf("%s: failed to malloc from \"%s\".\n", __func__, tomlString.u.s);}
}
void loadElemPara(Elem* elem, const toml_datum_t tomlPara) {
    // Req Condition
    if (elem == NULL) {printf("%s: elem not exists.\n", __func__); return;}
    if (elem->para != NULL) {printf("%s: elem.para not freed.\n", __func__); return;}

    // Opt Condition
    elem->para = tomlPara.ok ? strdup(tomlPara.u.s) : NULL;
}
void loadElemTexture(Elem* elem) {
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
void loadElemOther(Elem* elem, const toml_table_t* tomlElem, const int tomlElemId) {
    // Req Condition
    if (elem == NULL) {printf("%s: elem not exists.\n", __func__); return;}
    if (tomlElem == NULL) {printf("%s: tomlElem not exists.\n", __func__); return;}

    // loadElemAnchorFromToml
    const toml_datum_t tomlAnchor = toml_int_in(tomlElem, "anchor");
    elem->anchor = (Anchor)(tomlAnchor.ok ? tomlAnchor.u.i : 0);

    // loadElemFuncFromToml
    const toml_datum_t tomlFuncName = toml_string_in(tomlElem, "func");
    elem->func = tomlFuncName.ok ? getTrigFuncFromName(tomlFuncName.u.s) : NULL;

    // loadElemIdFromToml
    elem->id = tomlElemId;
}

void loadElem(Elem* elem, const toml_table_t* tomlElem, const int tomlElemId) {
    // Req Condition
    if (elem == NULL) {printf("%s: elem not exists.\n", __func__); return;}
    if (tomlElem == NULL) {printf("%s: tomlElem not exists.\n", __func__); return;}

    //
    killElem(elem);
    loadElemOther(elem, tomlElem, tomlElemId);
    loadElemGuide(elem, toml_array_in(tomlElem, "guide"));
    loadElemPara(elem, toml_string_in(tomlElem, "para"));
    loadElemString(elem, toml_string_in(tomlElem, "string"));
    loadElemTexture(elem);
    turnElemOn(elem);
}


// renew & draw
void renewElemDstRect(Elem* elem) {
    // Req Condition
    if (elem == NULL) {printf("%s: elem not exists.\n", __func__); return;}
    if (elem->on == false) {printf("%s: elem[%d] isn't on.\n", __func__, elem->id); return;}

    //
    const Anchor x = elem->anchor % 9;
    const Anchor y = elem->anchor / 9;
    SDL_GetTextureSize(elem->texture, &elem->dst_rect.w, &elem->dst_rect.h);
    elem->dst_rect.w *= elem->guide.w;
    elem->dst_rect.h *= elem->guide.h;
    Pixel cx = 0, cy = 0, dx = 0, dy = 0;
    switch (x / 3) {
        case 0: cx = menu.bck_rect.x                  ; break;
        case 1: cx = menu.bck_rect.x + menu.bck_rect.w / 2; break;
        case 2: cx = menu.bck_rect.x + menu.bck_rect.w    ; break;
        default: break;
    }
    switch (y / 3) {
        case 0: cy = menu.bck_rect.y                  ; break;
        case 1: cy = menu.bck_rect.y + menu.bck_rect.h / 2; break;
        case 2: cy = menu.bck_rect.y + menu.bck_rect.h    ; break;
        default: break;
    }
    switch (x % 3) {
        case 0: dx = -elem->dst_rect.w    ; break;
        case 1: dx = -elem->dst_rect.w / 2; break;
        case 2: dx =  0               ; break;
        default: break;
    }
    switch (y % 3) {
        case 0: dy = -elem->dst_rect.h    ; break;
        case 1: dy = -elem->dst_rect.h / 2; break;
        case 2: dy =  0               ; break;
        default: break;
    }
    elem->dst_rect.x = cx + dx + elem->guide.x;
    elem->dst_rect.y = cy + dy + elem->guide.y;
}
void renewElemState(Elem* elem) {
    // Req Condition
    if (elem == NULL) {printf("%s: elem not exists.\n", __func__); return;}
    if (elem->on == false) {printf("%s: elem[%d] isn't on.\n", __func__, elem->id); return;}

    //
    const bool mouseIn = mouseInRect(&elem->dst_rect);
    const bool mouseLeftIn = mouseLeftInRect(&elem->dst_rect);
    if (elem->state == PRESSED && mouseIn == true && mouseLeftIn == false) {elem->state = RELEASE;}
    else {
        if (mouseIn == true) {elem->state = mouseLeftIn ? PRESSED : INSIDE;}
        else {elem->state = OUTSIDE;}
    }
    if (elem->state == RELEASE && elem->func != NULL) {elem->func(elem->para);}
}

void renewElem(Elem* elem) {
    // Req Condition
    if (elem == NULL) {printf("%s: elem not exists.\n", __func__); return;}
    if (elem->on == false) {printf("%s: elem[%d] isn't on.\n", __func__, elem->id); return;}

    //
    renewElemDstRect(elem);
    renewElemState(elem);
}

void drawElem(const Elem* elem) {
    // Req Condition
    if (elem == NULL) {printf("%s: elem not exists.\n", __func__); return;}
    if (elem->on == false) {printf("%s: elem[%d] isn't on.\n", __func__, elem->id); return;}
    if (elem->texture == NULL) {printf("%s: elem[%d].texture not exists.\n", __func__, elem->id); return;}
    if (menu.renderer == NULL) {printf("%s: menu.renderer is NULL.\n", __func__); return;}

    //
    switch (elem->state) {
        case INSIDE: {DEBUG_DrawRect(menu.renderer, &elem->dst_rect); break;}
        case PRESSED:
        case RELEASE: {DEBUG_FillRect(menu.renderer, &elem->dst_rect); break;}
        case OUTSIDE:
        default: {break;}
    }
    SDL_RenderTexture(menu.renderer, elem->texture, &elem->src_rect, &elem->dst_rect);
}


#pragma endregion ELEM_FUNC ============================================================================================
#pragma region PAGE_FUNC ===============================================================================================


// load & kill
void loadPageName(Page* page, const char* name) {
    // Req Condition
    if (page == NULL) {printf("%s: page not exists.\n", __func__); return;}
    if (name == NULL) {printf("%s: name not exists.\n", __func__); return;}

    //
    page->name = strdup(name);

    // Req Condition
    if (page->name == NULL) {printf("%s: failed.\n", __func__);}
}
void loadPageElems(Page* page, const toml_array_t* tomlElems) {
    // Req Condition
    if (page == NULL) {printf("%s: page not exists.\n", __func__); return;}
    if (tomlElems == NULL) {printf("%s: tomlElems not exists.\n", __func__); return;}

    //
    const int tomlElemsVolume = toml_array_nelem(tomlElems);
    if (tomlElemsVolume >= PAGE_MAX_VOLUME) {printf("%s: tomlPage[%s].tomlElems overflows.\n", __func__, page->name); return;}
    //
    for (PageVolume i = 0; i < tomlElemsVolume; i++) {
        const toml_table_t* tomlElem = toml_table_at(tomlElems, i);
        // Opt Condition
        if (tomlElem != NULL) {loadElem(&page->elems[i], tomlElem, i);}
        else {turnElemOff(&page->elems[i]);}
    }
}

void loadPage(Page* page, const char* name, const toml_table_t* tomlPage) {
    // Req Condition
    if (page == NULL) {printf("%s: page not exists.\n", __func__); return;}
    if (name == NULL) {printf("%s: name not exists.\n", __func__); return;}
    if (tomlPage == NULL) {printf("%s: tomlPage not exists.\n", __func__); return;}

    //
    loadPageName(page, name);
    loadPageElems(page, toml_array_in(tomlPage, "elems"));
}


// renew & draw
void renewPage(Page* page) {
    // N-Condition
    if (page == NULL) {printf("%s: page not exists.\n", __func__); return;}

    // renewPageElems
    for (PageVolume i = 0; i < PAGE_MAX_VOLUME; i++) {
        if (page->elems[i].on) {renewElem(&page->elems[i]);}
    }
}

void drawPage(const Page* page) {
    // N-Condition
    if (page == NULL) {printf("%s: page not exists.\n", __func__); return;}

    // drawPageElems
    for (PageVolume i = 0; i < PAGE_MAX_VOLUME; i++) {
        if (page->elems[i].on) {drawElem(&page->elems[i]);}
    }
}


#pragma endregion PAGE_FUNC ============================================================================================
#pragma region MENU_FUNC ===============================================================================================


// menu.theme
void loadMenuThemeFromToml(const char* tomlPath) {
    // N-Condition
    FILE* tomlFile = fopen(tomlPath, "r");
    if (tomlFile == NULL) {printf("%s: failed to open \"%s\".\n", __func__, tomlPath); return;}

    // getMenuThemeFromToml
    toml_table_t* tomlMenuTheme = toml_parse_file(tomlFile, NULL, 0);  //  malloc
    fclose(tomlFile);
    if (tomlMenuTheme == NULL) {printf("%s: failed to read \"%s\".\n", __func__, tomlPath);}

    {
        // getMenuThemeFontPathFromToml
        const char* tomlFontPathKey = "font_path";
        const toml_datum_t tomlFontPath = toml_string_in(tomlMenuTheme, tomlFontPathKey);
        if (!tomlFontPath.ok) {printf("%s: no key \"%s\" in \"%s\".\n", __func__, tomlFontPathKey, tomlPath);}

        // getMenuThemeFontSizeFromToml
        const char* tomlFontSizeKey = "font_size";
        const toml_datum_t tomlFontSize = toml_int_in(tomlMenuTheme, tomlFontSizeKey);
        if (!tomlFontSize.ok) {printf("%s: no key \"%s\" in \"%s\".\n", __func__, tomlFontSizeKey, tomlPath);}

        // loadMenuThemeFont
        menu.theme.font = TTF_OpenFont(tomlFontPath.u.s, tomlFontSize.u.i);
        if (menu.theme.font == NULL) {printf("%s: failed from %s.\n", __func__, tomlFontPath.u.s);}

        // loadMenuThemeColor
        const char* tomlFontColorKey = "font_color";
        const toml_array_t* tomlFontColor = toml_array_in(tomlMenuTheme, tomlFontColorKey);
        if (tomlFontColor == NULL) {printf("%s: no key \"%s\" in \"%s\".\n", __func__, tomlFontSizeKey, tomlPath);}
        int color[4] = {};
        for (int i = 0; i < 4; i++) {
            const toml_datum_t c = toml_int_at(tomlFontColor, i);
            color[i] = c.ok ? c.u.i : 255;
        }
        menu.theme.color.r = color[0];
        menu.theme.color.g = color[1];
        menu.theme.color.b = color[2];
        menu.theme.color.a = color[3];
    }

    // freeTomlMenu
    toml_free(tomlMenuTheme);  // free
}
void killMenuTheme() {
    if (menu.theme.font != NULL) {
        TTF_CloseFont(menu.theme.font);
    }
}


// load
void loadMenuPagesFromToml(const char* tomlPath) {
    // N-Condition
    FILE* tomlFile = fopen(tomlPath, "r");
    if (tomlFile == NULL) {printf("%s: failed to open \"%s\".\n", __func__, tomlPath); return;}

    // getMenuFromToml
    toml_table_t* tomlMenu = toml_parse_file(tomlFile, NULL, 0);
    fclose(tomlFile);
    if (tomlMenu == NULL) {printf("%s: failed to read \"%s\".\n", __func__, tomlPath); return;}

    // getPageName
    const char pageRootName[] = "Root";
    const char pageEdgeName[] = "Edge";
    PageId pageId = 1;
    for (int i = 0; ; i++) {
        const char* pageName = toml_key_in(tomlMenu, i);
        if (pageName == NULL) {break;}

        // getPageFromToml
        const toml_table_t* tomlPage = toml_table_in(tomlMenu, pageName);

        // loadMenuPageFromToml
        if (tomlPage == NULL) {printf("%s: failed to get \"%s\" from tomlMenu[%p].\n", __func__, pageName, tomlMenu);}
        else if (strcmp(pageName, pageRootName) == 0) {loadPage(menu.pageRoot, pageName, tomlPage);}
        else if (strcmp(pageName, pageEdgeName) == 0) {loadPage(menu.pageEdge, pageName, tomlPage);}
        else {
            if (menu.pages[pageId] == NULL) {
                menu.pages[pageId] = malloc(sizeof(Page));
            }
            loadPage(menu.pages[pageId], pageName, tomlPage);
            pageId++;
        }
    }

    // freeTomlMenu
    toml_free(tomlMenu);
}
void loadMenu(SDL_Renderer* renderer) {
    menu.renderer = renderer;
    if (menu.pageRoot == NULL) {menu.pageRoot = malloc(sizeof(Page));}
    if (menu.pageEdge == NULL) {menu.pageEdge = malloc(sizeof(Page));}
    if (menu.pageRoot == NULL) {printf("%s: failed to malloc menu.page.Root.\n", __func__);}
    if (menu.pageEdge == NULL) {printf("%s: failed to malloc menu.page.Edge.\n", __func__);}
    loadMenuThemeFromToml("../menu_theme.toml");
    loadMenuPagesFromToml("../menu_pages.toml");
}


// get
Page* getMenuPageFromPathId(const PathId pathId) {
    return menu.pages[menu.path[pathId]];
}
Page* getMenuPageFromPageId(const PageId pageId) {
    return menu.pages[pageId];
}


// print
void printMenuPath() {
    printf("%s", menu.pageRoot->name);
    for (PathId i = 0; i < MENU_PATH_VOLUME; i++) {
        const Page* page = getMenuPageFromPathId(i);
        if (page == NULL) {continue;}
        printf("/%s", page->name);
    }
    printf("\n");
}


// renew
void renewMenuPathString() {
    strcpy(menu.pathString, menu.pageRoot->name);
    for (int i = 0; i < MENU_PATH_VOLUME && menu.path[i] != 0; i++) {
        strcat(menu.pathString, "/");
        strcat(menu.pathString, menu.pages[menu.path[i]]->name);
    }
    if (menu.path[MENU_PATH_VOLUME - 1] != 0) {
        strcat(menu.pathString, "/");
        strcat(menu.pathString, menu.pageEdge->name);
    }
}
void renewMenuPath() {
    bool need_clear = false;
    for (PathId i = 0; i < MENU_PATH_VOLUME; i++) {
        if (need_clear) {menu.path[i] = 0;}
        else if (getMenuPageFromPathId(i) == NULL) {need_clear = true;}
    }
}
void renewMenuPageNow() {
    // if pageRoot
    if (getMenuPageFromPathId(0) == NULL) {
        menu.pageNow = menu.pageRoot;
        return;
    }

    // else if pageOther
    for (PathId i = 0; i + 1 < MENU_PATH_VOLUME; i++) {
        if (getMenuPageFromPathId(i) != NULL && getMenuPageFromPathId(i+1) == NULL) {
            menu.pageNow = getMenuPageFromPathId(i);
            return;
        }
    }

    // else pageEdge
    menu.pageNow = menu.pageEdge;
}
void renewMenu() {
    static bool need_load = true;
    if (need_load || debug.on) {
        loadMenuPagesFromToml("../menu_pages.toml");
        need_load = false;
    }
    renewMenuPath();
    renewMenuPageNow();
    renewPage(menu.pageNow);
    // printf("%s\n", menu.pageNow->name);
    // printMenuPath();
    renewMenuPathString();
}


void drawMenu() {
    drawPage(menu.pageNow);
}


#pragma endregion MENU_FUNC ============================================================================================


#endif //MENU_H