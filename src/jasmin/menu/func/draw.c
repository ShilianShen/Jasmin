#include "func.h"


void MENU_DrawElem(const Elem* elem) {
    // Req Condition
    if (!ELEM_IfReady(elem)) {
        printf("%s: elem not ready.\n", __func__);
        return;
    }
    if (menu.renderer == NULL) {printf("%s: menu.renderer is NULL.\n", __func__); return;}

    //
    switch (elem->state) {
        case PRESSED:
        case RELEASE: {DEBUG_FillRect(&elem->dst_rect); break;}
        case INSIDE: {DEBUG_DrawRect(&elem->dst_rect); break;}
        case OUTSIDE:
            default: {break;}
    }
    SDL_RenderTexture(menu.renderer, elem->texture, &elem->src_rect, &elem->dst_rect);
}
void MENU_DrawPage(const Page* page) {
    // Req Condition
    if (page == NULL) {printf("%s: page not exists.\n", __func__); return;}

    //
    for (int i = 0; i < PAGE_ELEM_VOLUME; i++) {
        if (page->elems[i].on == true) {
            MENU_DrawElem(&page->elems[i]);
        }
    }
}
void MENU_DrawMenu() {
    MENU_DrawPage(menu.pageNow);
}


void MENU_Draw() {
    MENU_DrawMenu();
}