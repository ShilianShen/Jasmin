#ifndef MENU_H
#define MENU_H


#pragma region ELEM ====================================================================================================
#include "debug.h"


// first data type
typedef uint8_t Anchor;
typedef uint8_t ElemPara;
typedef char ElemString;
typedef enum {OUTSIDE, INSIDE, PRESSED, RELEASE, NUM_ELEM_STATES} ElemState;
typedef enum {PASS, FORWARD, BACKWARD, NUM_ELEM_FUNCS} ElemFunc;
const uint8_t ELEM_PATH_LENGTH = 32;


// second data type
typedef struct {

    // file
    Anchor anchor; SDL_FRect guide;
    ElemFunc func; ElemPara para;
    ElemString* string;

    // graphic
    SDL_Texture* texture; SDL_FRect src_rect, dst_rect;
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


const uint8_t MENU_VOLUME = 16, PATH_DEPTH = 16;


struct {
    PageId path[PATH_DEPTH];
    Page *pageRoot, *pageEdge, *pageNow, *pages[MENU_VOLUME];
    SDL_FRect bck_rect;
    struct {TTF_Font* font; SDL_Color color;} theme;
    SDL_Renderer* renderer;
} menu;


#pragma endregion MENU =================================================================================================
#pragma region ELEM_FUNC ===============================================================================================


SDL_Texture* getTextureFromElemString(const ElemString* elemString) {
    if (elemString == NULL) {
        printf("getTextureFromElemString: Elem-String[%p] not exist.\n", elemString);
        return NULL;
    }
    if (elemString[0] != 0 && elemString[1] != 0 && elemString[2] == 0) {
        printf("getTextureFromElemString: Illegal elem-string %s.\n", elemString);
        return NULL;
    }

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
        default: {
            texture = NULL;
            break;
        }
    }

    if (texture == NULL) {
        printf("getTextureFromElemString: Failed from %s.\n", elemString);
    }

    return texture;
}


void loadElemTexture(Elem* elem) {
    if (elem == NULL) {
        printf("loadElemTexture: elem[%p] not exists.\n", elem);
        return;
    }
    if (elem->string == NULL) {
        printf("loadElemTexture: elem[%p].string not exists.\n", elem);
        return;
    }
    if (elem->texture != NULL) {
        SDL_DestroyTexture(elem->texture);
        elem->texture = NULL;
    }

    elem->texture = getTextureFromElemString(elem->string);
    if (elem->texture == NULL) {
        printf("loadElemTexture: elem[%p].texture is NULL.\n", elem);
        return;
    }

    SDL_GetTextureSize(elem->texture, &elem->guide.w, &elem->guide.h);
    elem->src_rect = (SDL_FRect){0, 0, elem->guide.w, elem->guide.h};
}
void loadElem(Elem* elem, const Anchor anchor, const SDL_FRect guide, const ElemFunc func, const ElemPara para, const ElemString* string) {
    if (elem == NULL) {
        printf("loadElem: elem[%p] not exists.\n", elem);
        return;
    }

    elem->anchor = anchor;
    elem->guide = guide;
    elem->func = func;
    elem->para = para;

    elem->string = strdup(string);
    if (elem->string == NULL) {
        printf("loadElem: failed from elem[%p].\n", elem);
        return;
    }

    loadElemTexture(elem);
    if (elem->texture == NULL) {
        printf("loadElem: failed from elem.string[%p].\n", elem->string);
        return;
    }
}


void renewElemDstRect(Elem* elem) {
    if (elem == NULL) {return;}

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
    elem->dst_rect.x = cx + dx;
    elem->dst_rect.y = cy + dy;
}
void renewElemState(Elem* elem) {
    if (elem == NULL) return;
    elem->state = mouseInRect(&elem->dst_rect) ? INSIDE : OUTSIDE;
}
void renewElem(Elem* elem) {
    renewElemDstRect(elem);
    renewElemState(elem);
}


void drawElem(const Elem* elem) {
    if (elem == NULL) {return;}
    if (elem->texture == NULL) {return;}
    if (menu.renderer == NULL) {
        printf("drawElem: menu.renderer is NULL.\n");
        return;
    }

    if (elem->state == OUTSIDE) {DEBUG_DrawRect(menu.renderer, &elem->dst_rect);}
    else {DEBUG_FillRect(menu.renderer, &elem->dst_rect);}
    SDL_RenderTexture(menu.renderer, elem->texture, &elem->src_rect, &elem->dst_rect);
}


#pragma endregion ELEM_FUNC ============================================================================================
#pragma region PAGE_FUNC ===============================================================================================


void loadPage(Page* page) {
    for (PageVolume i = 0; i < PAGE_MAX_VOLUME; i++) {
        //
    }
}


void renewPage(Page* page) {
    if (page == NULL) {return;}
    for (PageVolume i = 0; i < PAGE_MAX_VOLUME; i++) {
        renewElem(&page->elems[i]);
    }
}


void drawPage(const Page* page) {
    if (page == NULL) {return;}
    for (PageVolume i = 0; i < PAGE_MAX_VOLUME; i++) {
        drawElem(&page->elems[i]);
    }
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


// menu.pageRoot
void loadMenuPageRoot() {
    Page* pageRoot = malloc(sizeof(Page));
    pageRoot->name = strdup("Root");
    loadElem(&pageRoot->elems[0], 20, (SDL_FRect){0, 0, 0, 0}, 0, 0, "t-Root");
    menu.pageRoot = pageRoot;
}


// menu.pageEdge
void loadMenuEdge() {}
void loadMenu(SDL_Renderer* renderer) {
    menu.renderer = renderer;
    loadMenuTheme();
    loadMenuPageRoot();
}
void renewMenu() {
    if (menu.path[0] == 0) {menu.pageNow = menu.pageRoot;}
    else {
        for (int i = 0; i + 1 < MENU_VOLUME; i++) {
            if (menu.path[i] != 0 && menu.path[i + 1] == 0) {
                menu.pageNow = menu.pages[menu.path[i]];
                for (int j = i + 1; j < PAGE_MAX_VOLUME; j++) {
                    menu.path[i] = 0;
                }
                break;
            }
        }
    }
    if (menu.path[MENU_VOLUME - 1] != 0) {menu.pageNow = menu.pageEdge;}
    menu.pageNow = menu.pageRoot;
    // printf("%s\n", menu.pageNow->name);
    renewPage(menu.pageNow);
}


void drawMenu() {
    menu.pageNow = menu.pageRoot;
    drawPage(menu.pageNow);
};


#pragma endregion MENU_FUNC ============================================================================================


#endif //MENU_H