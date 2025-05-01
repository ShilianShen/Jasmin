#ifndef MENU_H
#define MENU_H


#pragma region ELEM ====================================================================================================


// first data type
typedef uint8_t Anchor;
typedef uint8_t ElemPara;
typedef char ElemString;
typedef enum {OUTSIDE, INSIDE, PRESSED, RELEASE, NUM_ELEM_STATES} ElemState;
typedef enum {PASS, FORWARD, BACKWARD, NUM_ELEM_FUNCS} ElemFunc;
const uint8_t ELEM_PATH_LENGTH = 32;


// second data type
typedef struct {
    bool on;

    // file
    Anchor anchor; SDL_FRect guide;
    ElemFunc func; ElemPara para;
    ElemString* string;

    // graphic
    SDL_Texture* texture;

    // renewable
    SDL_FRect src_rect, dst_rect;
    ElemState state;

} Elem;


#pragma endregion ELEM =================================================================================================
#pragma region PAGE ====================================================================================================


typedef uint8_t PageId;
typedef char PageName;
typedef int PageVolume;
const PageVolume PAGE_MAX_VOLUME = 32;


typedef struct {
    PageName* name; PageId id;
    Elem elems[PAGE_MAX_VOLUME];
} Page;


#pragma endregion PAGE =================================================================================================
#pragma region MENU ====================================================================================================


const uint8_t MENU_MEX_VOLUME = 16, PATH_DEPTH = 16;


struct {
    PageId path[PATH_DEPTH];
    Page *pageRoot, *pageEdge, *pageNow, *pages[MENU_MEX_VOLUME];
    SDL_FRect bck_rect;
    struct {TTF_Font* font; SDL_Color color;} theme;
    SDL_Renderer* renderer;
} menu;


#pragma endregion MENU =================================================================================================
#pragma region ELEM_FUNC ===============================================================================================


// get
/**
 * @brief 从elemString加载texture, 返回指针.
 * @details 会检查elemString风格是否正确
 */
SDL_Texture* getTextureFromElemString(const ElemString* elemString) {
    // N-Condition
    if (elemString == NULL) {printf("%s: elemString not exist.\n", __func__); return NULL;}
    if (strlen(elemString) <= 2) {printf("%s: elemString[%s] isn't legal.\n", __func__, elemString); return NULL;}

    //
    SDL_Texture* texture = NULL;
    switch (elemString[0]) {
        case 't': {
            texture = TXT_LoadTexture(menu.renderer, menu.theme.font, elemString+2, menu.theme.color);
            break;
        }
        case 'f': {
            texture = IMG_LoadTexture(menu.renderer, elemString+2);
            break;
        }
        default: {texture = NULL; break;}
    }
    if (texture == NULL) {printf("%s: failed from \"%s\".\n", __func__, elemString);}

    return texture;
}


// load
/**
 * @brief 从tomlElem加载属性到elem
 * @details 检查参数, 如果tomlElem有属性缺失, 会用使用默认值, 而不会视为错误
 */
void loadElemFromToml(Elem* elem, const toml_table_t* tomlElem) {
    // N-Condition
    if (elem == NULL) {printf("%s: elem not exists.\n", __func__); return;}
    if (tomlElem == NULL) {printf("%s: tomlElem not exists.\n", __func__); return;}

    // loadElemAnchorFromToml
    const toml_datum_t tomlAnchor = toml_int_in(tomlElem, "anchor");
    elem->anchor = (Anchor)(tomlAnchor.ok ? tomlAnchor.u.i : 0);

    // loadElemGuideFromToml
    const toml_array_t* tomlGuide = toml_array_in(tomlElem, "guide");
    if (tomlGuide != NULL) {
        const toml_datum_t tomlGuideX = toml_double_at(tomlGuide, 0);
        const toml_datum_t tomlGuideY = toml_double_at(tomlGuide, 1);
        elem->guide.x = (float)(tomlGuideX.ok ? tomlGuideX.u.d : 0);
        elem->guide.y = (float)(tomlGuideY.ok ? tomlGuideY.u.d : 0);
    }
    else {elem->guide.x = elem->guide.y = 0;}

    // loadElemStringFromToml
    const toml_datum_t tomlString = toml_string_in(tomlElem, "string");
    if (elem->string != NULL) {free(elem->string);}
    elem->string = strdup(tomlString.ok ? tomlString.u.s : "");

    // loadElemFuncFromToml
    elem->func = 0;

    // loadElemParaFromToml
    elem->para = 0;

    // turn on
    elem->on = true;
}
/**
 * @brief 从C语言内部加载属性到elem
 */
void loadElemFromC(Elem* elem, const Anchor anchor, const SDL_FRect guide, const ElemFunc func, const ElemPara para, const ElemString* string) {
    // N-Condition
    if (elem == NULL) {printf("%s: elem not exists.\n", __func__); return;}

    // loadElemOtherFromC
    elem->anchor = anchor;
    elem->guide = guide;
    elem->func = func;
    elem->para = para;

    // loadElemStringFromC
    if (string != NULL) {elem->string = strdup(string);}
    else {printf("%s: string not exists.\n", __func__); return;}
    if (elem->string == NULL) {printf("%s: failed \"%s\" into elem[%p].string.\n", __func__, string, elem);}

    // turn on
    elem->on = true;
}


// renew
/**
 * @bried 根据string, 刷新texture
 */
void renewElemTexture(Elem* elem) {
    // P-Condition
    if (elem == NULL) {return;}
    if (elem->on == false) {return;}

    // N-Condition
    if (elem->string == NULL) {printf("%s: elem[%p].string not exists.\n", __func__, elem); return;}

    //
    if (elem->texture != NULL) {SDL_DestroyTexture(elem->texture); elem->texture = NULL;}
    elem->texture = getTextureFromElemString(elem->string);
    if (elem->texture == NULL) {printf("%s: failed from elem[%p].texture.\n", __func__, elem); return;}

    //
    SDL_GetTextureSize(elem->texture, &elem->guide.w, &elem->guide.h);
    elem->src_rect = (SDL_FRect){0, 0, elem->guide.w, elem->guide.h};
}
/**
 * @brief 自动更新绘制矩形的参数
 */
void renewElemDstRect(Elem* elem) {
    // P-Condition
    if (elem == NULL) {return;}
    if (elem->on == false) {return;}

    //
    const Anchor x = elem->anchor % 9;
    const Anchor y = elem->anchor / 9;
    elem->dst_rect.w = elem->guide.w;
    elem->dst_rect.h = elem->guide.h;
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
/**
 *
 * @brief 根据鼠标位置和矩形判断elemState
 */
void renewElemState(Elem* elem) {
    // P-Condition
    if (elem == NULL) return;
    if (elem->on == false) {return;}

    //
    elem->state = mouseInRect(&elem->dst_rect) ? INSIDE : OUTSIDE;
}
/**
 * @brief renewFunc的集成
 */
void renewElem(Elem* elem) {
    // P-Condition
    if (elem == NULL) {return;}
    if (elem->on == false) {return;}

    //
    renewElemTexture(elem);
    renewElemDstRect(elem);
    renewElemState(elem);
}


// draw
/**
 * @brief 绘制
 */
void drawElem(const Elem* elem) {
    // P-Condition
    if (elem == NULL) {return;}
    if (elem->on == false) {return;}

    // N-Condition
    if (elem->texture == NULL) {printf("%s: elem[%p].texture not exists.\n", __func__, elem); return;}
    if (menu.renderer == NULL) {printf("%s: menu.renderer is NULL.\n", __func__); return;}

    //
    if (elem->state == OUTSIDE) {DEBUG_DrawRect(menu.renderer, &elem->dst_rect);}
    else {DEBUG_FillRect(menu.renderer, &elem->dst_rect);}

    SDL_RenderTexture(menu.renderer, elem->texture, &elem->src_rect, &elem->dst_rect);
}


#pragma endregion ELEM_FUNC ============================================================================================
#pragma region PAGE_FUNC ===============================================================================================


// load
/**
 * @brief
 */
void loadPageFromToml(Page* page, const char* name, const toml_table_t* tomlPage) {
    // N-Condition
    if (page == NULL) {printf("%s: page not exists.\n", __func__); return;}
    if (name == NULL) {printf("%s: name not exists.\n", __func__); return;}
    if (tomlPage == NULL) {printf("%s: tomlPage not exists.\n", __func__); return;}

    // loadPageNameFromToml
    page->name = strdup(name);

    // loadPageElemsFromToml
    const toml_array_t* tomlElems = toml_array_in(tomlPage, "elems");
    if (tomlElems != NULL) {
        for (PageVolume i = 0; i < PAGE_MAX_VOLUME; i++) {
            Elem* elem = &page->elems[i];
            const toml_table_t* tomlElem = toml_table_at(tomlElems, i);
            if (tomlElem != NULL) {loadElemFromToml(elem, tomlElem);}
            else {elem->on = false;}
        }
    }
}


// renew
/**
 * @brief
 * @param page
 */
void renewPage(Page* page) {
    // N-Condition
    if (page == NULL) {printf("%s: page not exists.\n", __func__); return;}

    //
    for (PageVolume i = 0; i < PAGE_MAX_VOLUME; i++) {renewElem(&page->elems[i]);}
}


// draw
/**
 * @brief
 * @param page
 */
void drawPage(const Page* page) {
    // N-Condition
    if (page == NULL) {printf("%s: page not exists.\n", __func__); return;}

    //
    for (PageVolume i = 0; i < PAGE_MAX_VOLUME; i++) {drawElem(&page->elems[i]);}
}


#pragma endregion PAGE_FUNC ============================================================================================
#pragma region MENU_FUNC ===============================================================================================


// menu.theme
void loadMenuTheme() {
    // font
    const char fontPath[] = "../fonts/Courier New.ttf";
    menu.theme.font = TTF_OpenFont(fontPath, 40);
    if (menu.theme.font == NULL) {
        printf("loadMenuTheme: Failed in %s.\n", fontPath);
    }

    // color
    menu.theme.color.r = 255;
    menu.theme.color.g = 255;
    menu.theme.color.b = 255;
    menu.theme.color.a = 255;
}
void killMenuTheme() {
    if (menu.theme.font != NULL) {
        TTF_CloseFont(menu.theme.font);
    }
}


void loadMenuPageRoot(const char* pageRootName, const toml_table_t* tomlPageRoot) {
    // ?-Condition
    if (menu.pageRoot == NULL) {menu.pageRoot = malloc(sizeof(Page));}

    // N-Condition
    if (menu.pageRoot == NULL) {printf("%s: failed to malloc menu.page.Root.\n", __func__);}

    // loadMenuPageRootFromToml
    if (tomlPageRoot != NULL) {loadPageFromToml(menu.pageRoot, pageRootName, tomlPageRoot); return;}

    // loadMenuPageRootFromC
    menu.pageRoot->name = strdup(pageRootName);
    loadElemFromC(&menu.pageRoot->elems[0], 40, (SDL_FRect){0, 0}, 0, 0, "t-Root");
}
void loadMenuPageEdge(const char* pageEdgeName, const toml_table_t* tomlPageEdge) {
    // ?-Condition
    if (menu.pageEdge == NULL) {menu.pageEdge = malloc(sizeof(Page));}

    // N-Condition
    if (menu.pageEdge == NULL) {printf("%s: failed to malloc menu.page.Edge.\n", __func__);}

    // loadMenuPageEdgeFromToml
    if (tomlPageEdge != NULL) {loadPageFromToml(menu.pageEdge, pageEdgeName, tomlPageEdge); return;}

    // loadMenuPageEdgeFromC
    menu.pageEdge->name = strdup(pageEdgeName);
    loadElemFromC(&menu.pageEdge->elems[0], 40, (SDL_FRect){0, 0}, 0, 0, "t-Edge");
}
void loadMenuFromToml(const char* tomlPath) {
    static char pageRootKey[] = "Root";
    static char pageEdgeKey[] = "Edge";
    // N-Condition
    FILE* tomlFile = fopen(tomlPath, "r");
    if (tomlFile == NULL) {printf("%s: failed to open \"%s\".\n", __func__, tomlPath); return;}

    toml_table_t* tomlMenu = toml_parse_file(tomlFile, NULL, 0);
    fclose(tomlFile);
    if (tomlMenu == NULL) {printf("%s: failed to read \"%s\".\n", __func__, tomlPath); return;}

    const toml_table_t* tomlPageRoot = toml_table_in(tomlMenu, pageRootKey);
    if (tomlPageRoot == NULL) {printf("%s: key=\"%s\" not exists in \"%s\".\n", __func__, pageRootKey, tomlPath);}
    loadMenuPageRoot(pageRootKey, tomlPageRoot);

    const toml_table_t* tomlPageEdge = toml_table_in(tomlMenu, pageEdgeKey);
    if (tomlPageEdge == NULL) {printf("%s: key=\"%s\" not exists in \"%s\".\n", __func__, pageEdgeKey, tomlPath);}
    loadMenuPageEdge(pageEdgeKey, tomlPageEdge);

    toml_free(tomlMenu);
}
void loadMenu(SDL_Renderer* renderer) {
    menu.renderer = renderer;
    loadMenuTheme();
    loadMenuFromToml("../menu.toml");
}


void printMenuPath() {
    printf("\rPATH_%s", menu.pageRoot->name);
    for (int i = 0; i < PATH_DEPTH && menu.path[i] != 0; i++) {
        printf("\\%s", menu.pages[menu.path[i]]->name);
    }
    if (menu.path[PATH_DEPTH - 1] != 0) {
        printf("\\%s", menu.pageEdge->name);
    }
}


void renewMenu() {
    loadMenuFromToml("../menu.toml");
    if (menu.path[0] == 0) {menu.pageNow = menu.pageRoot;}
    else {
        for (int i = 0; i + 1 < MENU_MEX_VOLUME; i++) {
            if (menu.path[i] != 0 && menu.path[i + 1] == 0) {
                menu.pageNow = menu.pages[menu.path[i]];
                for (int j = i + 1; j < PAGE_MAX_VOLUME; j++) {
                    menu.path[i] = 0;
                }
                break;
            }
        }
    }
    if (menu.path[MENU_MEX_VOLUME - 1] != 0) {menu.pageNow = menu.pageEdge;}
    menu.pageNow = menu.pageRoot;
    // printf("%s\n", menu.pageNow->name);
    renewPage(menu.pageNow);
    // printMenuPath();
}


void drawMenu() {
    menu.pageNow = menu.pageRoot;
    drawPage(menu.pageNow);
}


#pragma endregion MENU_FUNC ============================================================================================


#endif //MENU_H