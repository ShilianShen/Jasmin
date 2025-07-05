#include "func.h"


void ELEM_Init(Elem* elem) {
    // Req Condition
    if (elem == NULL) {printf("%s: elem not exists.\n", __func__); return;}

    //
    *elem = (Elem){0};
}
void PAGE_Init(Page* page) {
    // Req Condition
    if (page == NULL) {printf("%s: page not exists.\n", __func__); return;}

    //
    *page = (Page){0};
    for (int i = 0; i < PAGE_ELEM_VOLUME; i++) {
        ELEM_Init(&page->elems[i]);
    }
}
void MENU_InitMenu(SDL_Renderer* renderer) {
    // Req Condition
    if (renderer == NULL) {
        printf("%s: renderer not exists.\n", __func__);
        return;
    }
    if (menu.pageRoot != NULL) {
        printf("%s: menu.pageRoot shouldn't exist.\n", __func__);
        return;
    }
    if (menu.pageEdge != NULL) {
        printf("%s: menu.pageEdge shouldn't exist.\n", __func__);
        return;
    }
    for (int i = 1; i < MENU_PAGE_VOLUME; i++) {
        if (menu.pages[i] != NULL) {
            printf("%s: menu.pages[%d] shouldn't exist.\n", __func__, i);
            return;
        }
    }

    //
    menu = (Menu){0};
    menu.renderer = renderer;
    menu.pageRoot = malloc(sizeof(Page)); // malloc pageRoot
    menu.pageEdge = malloc(sizeof(Page)); // malloc pageEdge
    PAGE_Init(menu.pageRoot);
    PAGE_Init(menu.pageEdge);
    for (int i = 1; i < MENU_PAGE_VOLUME; i++) {
        if (menu.pages[i] == NULL) {
            menu.pages[i] = malloc(sizeof(Page)); // malloc pages
            PAGE_Init(menu.pages[i]);
        }
    }
}
void MENU_Init(SDL_Renderer* renderer) {
    MENU_InitMenu(renderer);
}