#ifndef INIT_H
#define INIT_H


void MENU_InitElem(Elem* elem) {
    // Req Condition
    if (elem == NULL) {printf("%s: elem not exists.\n", __func__); return;}

    //
    *elem = (Elem){0};
}
void MENU_InitPage(Page* page) {
    // Req Condition
    if (page == NULL) {printf("%s: page not exists.\n", __func__); return;}

    //
    *page = (Page){0};
    page->id = -1;
}

void MENU_InitMenu(SDL_Renderer* renderer) {
    // Req Condition
    if (renderer == NULL) {printf("%s: renderer not exists.\n", __func__); return;}

    //
    menu.renderer = renderer;
    if (menu.pageRoot == NULL) {menu.pageRoot = malloc(sizeof(Page));}
    if (menu.pageEdge == NULL) {menu.pageEdge = malloc(sizeof(Page));}
    MENU_InitPage(menu.pageRoot);
    MENU_InitPage(menu.pageEdge);
    for (int i = 1; i < MENU_PAGE_VOLUME; i++) {
        if (menu.pages[i] == NULL) {
            menu.pages[i] = malloc(sizeof(Page));
            MENU_InitPage(menu.pages[i]);
        }
    }
}


#endif //INIT_H
