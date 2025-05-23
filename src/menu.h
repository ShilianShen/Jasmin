#ifndef MENU_H
#define MENU_H


#pragma region TRIG ====================================================================================================
/*
 * Trig是Menu中调用函数的手段, 涉及的数据类型有Trig, TrigName, TrigFunc, TrigPara和变量trig_set.
 * TrigFunc函数和TrigPara外来参数两者一起描述调用函数的具体细节, 即实际发生trigFunc(trigPara).
 * TrigName应该是TrigFunc的文本解释.
 * TrigName和TrigFunc共同构成Trig.
 * 所有的Trig构成有限的trig_set.
 * trig_set是检索范围, TrigName和TrigFunc是检索关键字.
 * TrigName和TrigFunc被定义双射, 即可以用TrigName在trig_set内查找到TrigFunc或相反, 同样是Trig内的唯一检索手段.
 *
 * 流: toml -> (TrigName, TrigPara) -(trig_set)-> (TrigFunc, TrigPara) -> TrigFunc(TrigPara)
 *
 * Q: 为什么用TrigFunc和TrigPara两个对象来描述而不是只用一个对象?
 * A: 如果用一个对象来描述, 那么在能forward到n个page时, 需要储存的空间是n*sizeof(forward), 但是分成两个参数就不需要了.
 */


// datatype
typedef char* TrigPara;
typedef char* TrigName;
typedef void (*TrigFunc)(TrigPara);
typedef struct {const TrigName name; const TrigFunc func;} Trig;


void trigFuncPass(TrigPara);
void trigFuncForward(TrigPara);
void trigFuncBackward(TrigPara);
void trigFuncClear(TrigPara);


const Trig trig_set[] = {
    {"pass", trigFuncPass},
    {"forward", trigFuncForward},
    {"backward", trigFuncBackward},
    {"clear", trigFuncClear},
    {NULL, NULL}
};


TrigFunc findTrigFuncFromName(const TrigName name) {
    // Req Condition
    if (name == NULL) {printf("%s: name not exists.\n", __func__); return NULL;}

    //
    for (int i = 0; trig_set[i].name != NULL; i++) {
        if (strcmp(trig_set[i].name, name) == 0) {
            return trig_set[i].func;
        }
    }
    return NULL;
}
TrigName findTrigNameFromFunc(const TrigFunc func) {
    // Req Condition
    if (func == NULL) {printf("%s: func not exists.\n", __func__); return NULL;}

    //
    for (int i = 0; trig_set[i].func != NULL; i++) {
        if (trig_set[i].func == func) {
            return trig_set[i].name;
        }
    }
    return NULL;
}


#pragma endregion TRIG  ================================================================================================
#pragma region ELEM ====================================================================================================
/*
 * Elem是图形显示的单位, 同时和一个TrigFunc(可以是NULL)绑定, 以实现复杂的功能.
 */

typedef uint8_t Anchor;
typedef int ElemId; // uint?
typedef char* ElemStr;
typedef enum {OUTSIDE, INSIDE, PRESSED, RELEASE, NUM_ELEM_STATES} ElemState;


typedef struct {
    // file
    ElemId id;  // id意味着page的第几个元素, 从1开始, id=0意味着未初始化
    Anchor anchor;
    SDL_FRect guide;
    TrigFunc func;
    TrigPara para; // malloc
    ElemStr string; // malloc
    SDL_Texture* texture; // malloc
    // renewable
    SDL_FRect src_rect, dst_rect;
    ElemState state;
} Elem;


#pragma endregion ELEM =================================================================================================
#pragma region PAGE ====================================================================================================


typedef char PageName;
typedef int PageId;
typedef int PageVolume;
const PageVolume PAGE_MAX_VOLUME = 32;


typedef struct {
    PageId id;
    PageName* name;  // malloc
    Elem elems[PAGE_MAX_VOLUME];
} Page;


#pragma endregion PAGE =================================================================================================
#pragma region MENU ====================================================================================================


typedef int PathId;
const PageId MENU_PAGE_VOLUME = 16;
const PathId MENU_PATH_VOLUME = 6;
const char MENU_THEME_PATH[] = "../menu/theme.toml";
const char MENU_PAGES_PATH[] = "../menu/pages.toml";
const char MENU_ROOT_NAME[] = "Root";
const char MENU_EDGE_NAME[] = "Edge";


struct {
    SDL_Renderer* renderer;  // malloc???
    SDL_FRect bck_rect;

    PageId path[MENU_PATH_VOLUME];
    char pathString[128];

    Page *pageRoot, *pageEdge, *pageNow, *pages[MENU_PAGE_VOLUME];  // malloc

    struct {
        TTF_Font* font;  // malloc
        SDL_Color color;
    } theme;
} menu;


#pragma endregion MENU =================================================================================================
#pragma region ELEM_FUNC ===============================================================================================


// other
SDL_Texture* getTextureFromElemString(const ElemStr elemStr) {
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


// test
bool testElem(const Elem* elem, const char* string) {
    if (elem == NULL) {printf("%s: elem not exists.\n", string); return false;}
    if (elem->id < 0) {printf("%s: elem id is negative.\n", string); return false;}
    if (elem->string == NULL) {printf("%s: elem[%d].string not exists.\n", string, elem->id); return false;}
    if (elem->texture == NULL) {printf("%s: elem[%d].texture not exists.\n", string, elem->id); return false;}
    return true;
}


// init
void initElem(Elem* elem) {
    // Req Condition
    if (elem == NULL) {printf("%s: elem not exists.\n", __func__); return;}

    //
    *elem = (Elem){0};
}


// load
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
    elem->func = tomlFuncName.ok ? findTrigFuncFromName(tomlFuncName.u.s) : NULL;

    // loadElemIdFromToml
    elem->id = tomlElemId;
}

void loadElem(Elem* elem, const toml_table_t* tomlElem, const int tomlElemId) {
    // Req Condition
    if (elem == NULL) {printf("%s: elem not exists.\n", __func__); return;}
    if (tomlElem == NULL) {printf("%s: tomlElem not exists.\n", __func__); return;}

    //
    loadElemOther(elem, tomlElem, tomlElemId);
    loadElemGuide(elem, toml_array_in(tomlElem, "guide"));
    loadElemPara(elem, toml_string_in(tomlElem, "para"));
    loadElemString(elem, toml_string_in(tomlElem, "string"));
    loadElemTexture(elem);
}


// kill
void killElem(Elem* elem) {
    // Req Condition
    if (elem == NULL) {printf("%s: elem not exists.\n", __func__); return;}

    //
    if (elem->string != NULL) {free(elem->string); elem->string = NULL;}
    if (elem->para != NULL) {free(elem->para); elem->para = NULL;}
    if (elem->texture != NULL) {SDL_DestroyTexture(elem->texture); elem->texture = NULL;}
}


// renew
void renewElemDstRect(Elem* elem) {
    // Req Condition
    if (!testElem(elem, __func__)) {return;}

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
    if (!testElem(elem, __func__)) {return;}

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
    if (!testElem(elem, __func__)) {return;}

    //
    renewElemDstRect(elem);
    renewElemState(elem);
}


// draw
void drawElem(const Elem* elem) {
    // Req Condition
    if (!testElem(elem, __func__)) {return;}
    if (menu.renderer == NULL) {printf("%s: menu.renderer is NULL.\n", __func__); return;}

    //
    switch (elem->state) {
        case PRESSED:
        case RELEASE: {DEBUG_FillRect(&elem->dst_rect);}
        case INSIDE: {DEBUG_DrawRect(&elem->dst_rect); break;}
        case OUTSIDE:
        default: {break;}
    }
    SDL_RenderTexture(menu.renderer, elem->texture, &elem->src_rect, &elem->dst_rect);
}


#pragma endregion ELEM_FUNC ============================================================================================
#pragma region PAGE_FUNC ===============================================================================================


// test
bool testPage(const Page* page, const char* string) {
    if (page == NULL) {if (string != NULL) {printf("%s: page not exists.\n", string);} return false;}
    if (page->name == NULL) {if (string != NULL) {printf("%s: name not exists.\n", string);} return false;}
    return true;
}


// init
void initPage(Page* page) {
    // Req Condition
    if (page == NULL) {printf("%s: page not exists.\n", __func__); return;}

    //
    *page = (Page){0};
    page->id = -1;
}


// load
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
            loadElem(&page->elems[i], tomlElem, i+1);
        }
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


// kill
void killPage(Page* page) {
    // Req Condition
    if (page == NULL) {printf("%s: page not exists.\n", __func__); return;}

    //
    if (page->name != NULL) {free(page->name); page->name = NULL;}
    for (int i = 0; i < PAGE_MAX_VOLUME; i++) {killElem(&page->elems[i]);}
}


// renew
void renewPage(Page* page) {
    // Req Condition
    if (page == NULL) {printf("%s: page not exists.\n", __func__); return;}

    //
    for (PageVolume i = 0; i < PAGE_MAX_VOLUME; i++) {
        if (page->elems[i].id != 0) {renewElem(&page->elems[i]);}
    }
}


// draw
void drawPage(const Page* page) {
    // Req Condition
    if (page == NULL) {printf("%s: page not exists.\n", __func__); return;}

    //
    for (PageVolume i = 0; i < PAGE_MAX_VOLUME; i++) {
        if (page->elems[i].id != 0) {drawElem(&page->elems[i]);}
    }
}


#pragma endregion PAGE_FUNC ============================================================================================
#pragma region MENU_FUNC ===============================================================================================


// other
toml_table_t* getToml(const char* tomlPath) {
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
Page* getMenuPageFromPathId(const PathId pathId) {
    return menu.pages[menu.path[pathId]];
}
Page* getMenuPageFromPageId(const PageId pageId) {
    return menu.pages[pageId];
}


// init
void initMenu() {
    if (menu.pageRoot == NULL) {menu.pageRoot = malloc(sizeof(Page));}
    if (menu.pageEdge == NULL) {menu.pageEdge = malloc(sizeof(Page));}
    initPage(menu.pageRoot);
    initPage(menu.pageEdge);
    for (int i = 1; i < MENU_PAGE_VOLUME; i++) {
        if (menu.pages[i] == NULL) {
            menu.pages[i] = malloc(sizeof(Page));
            initPage(menu.pages[i]);
        }
    }
}


// load
void loadMenuThemeFont(const toml_datum_t tomlFontPath, const toml_datum_t tomlFontSize) {
    // Req Condition
    if (menu.theme.font != NULL) {printf("%s: theme.font not free.\n", __func__); return;}
    if (!tomlFontPath.ok) {printf("%s: tomlFontPath not exists.\n", __func__); return;}
    if (!tomlFontSize.ok) {printf("%s: tomlFontSize not exists.\n", __func__); return;}

    //
    menu.theme.font = TTF_OpenFont(tomlFontPath.u.s, tomlFontSize.u.i);
    if (menu.theme.font == NULL) {printf("%s: failed from %s.\n", __func__, tomlFontPath.u.s);}
}
void loadMenuThemeColor(const toml_array_t* tomlColor) {
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
void loadMenuTheme(const char* tomlPath) {
    // Req Condition
    toml_table_t* tomlMenuTheme = getToml(tomlPath);
    if (tomlMenuTheme == NULL) {printf("%s: failed from \"%s\".\n", __func__, tomlPath); return;}

    //
    loadMenuThemeFont(
        toml_string_in(tomlMenuTheme, "font_path"),
        toml_int_in(tomlMenuTheme, "font_size"));
    loadMenuThemeColor(toml_array_in(tomlMenuTheme, "font_color"));

    //
    toml_free(tomlMenuTheme);
}
void loadMenuPages(const char* tomlPath) {
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
        else if (strcmp(pageName, MENU_ROOT_NAME) == 0) {loadPage(menu.pageRoot, pageName, tomlPage);}
        else if (strcmp(pageName, MENU_EDGE_NAME) == 0) {loadPage(menu.pageEdge, pageName, tomlPage);}
        else {
            if (menu.pages[pageId] == NULL) {
                menu.pages[pageId] = malloc(sizeof(Page));
            }
            loadPage(menu.pages[pageId], pageName, tomlPage);
            pageId++;
        }
    }

    //
    toml_free(tomlMenu); // end malloc
}
void loadMenu() {
    // Req Condition
    if (menu.renderer == NULL) {printf("%s: renderer not exists.\n", __func__); return;}

    loadMenuTheme(MENU_THEME_PATH);
    loadMenuPages(MENU_PAGES_PATH);
}


// kill
void killMenuTheme() {
    if (menu.theme.font != NULL) {
        TTF_CloseFont(menu.theme.font);
        menu.theme.font = NULL;
    }
}
void killMenu() {
    killPage(menu.pageRoot);
    killPage(menu.pageEdge);
    for (int i = 0; i < MENU_PAGE_VOLUME; i++) {
        if (menu.pages[i] != NULL) {
            killPage(menu.pages[i]);
        }
    }
    killMenuTheme();
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
        need_load = false;
    }
    renewMenuPath();
    renewMenuPageNow();
    renewPage(menu.pageNow);
    // printf("%s\n", menu.pageNow->name);
    // printMenuPath();
    renewMenuPathString();
}


// draw
void drawMenu() {
    drawPage(menu.pageNow);
}


#pragma endregion MENU_FUNC ============================================================================================
#pragma region TRIG_FUNC ===============================================================================================


void trigFuncPass(const TrigPara para) {}
void trigFuncForward(const TrigPara pageName) {
    ma_engine_play_sound(&maEngine, "../sound effects/ring01.wav", NULL);

    // getPageId
    PageId pageId = 0;
    for (PageId i = 0; i < MENU_PAGE_VOLUME; i++) {
        if (!testPage(menu.pages[i], NULL)) {continue;}
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


#pragma endregion TRIG_FUNC ============================================================================================


#endif //MENU_H