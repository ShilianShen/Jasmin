#ifndef DRAW_H
#define DRAW_H


void MENU_DrawElem(const Elem* elem) {
    // Req Condition
    if (!testElem(elem, __func__)) {return;}
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
    for (PageVolume i = 0; i < PAGE_MAX_VOLUME; i++) {
        if (page->elems[i].id != 0) {MENU_DrawElem(&page->elems[i]);}
    }
}
void MENU_DrawMenu() {
    MENU_DrawPage(menu.pageNow);
}



#endif //DRAW_H
