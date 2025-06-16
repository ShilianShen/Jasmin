#ifndef RENEW_H
#define RENEW_H


// renew
void MENU_RenewElemDstRect(Elem* elem) {
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
void MENU_RenewElemState(Elem* elem) {
    // Req Condition
    if (!testElem(elem, __func__)) {return;}

    //
    const bool mouseIn = mouseInRect(&elem->dst_rect);
    const bool mouseLeftIn = mouseLeftInRect(&elem->dst_rect);
    if (elem->state == PRESSED) {
        DEBUG_SendMessage("Elem: %d", elem->id);
    }
    if (elem->state == PRESSED && mouseIn == true && mouseLeftIn == false) {elem->state = RELEASE;}
    else {
        if (mouseIn == true) {elem->state = mouseLeftIn ? PRESSED : INSIDE;}
        else {elem->state = OUTSIDE;}
    }
    if (elem->state == RELEASE && elem->func != NULL) {elem->func(elem->para);}
}

void MENU_RenewElem(Elem* elem) {
    // Req Condition
    if (!testElem(elem, __func__)) {return;}

    //
    MENU_RenewElemDstRect(elem);
    MENU_RenewElemState(elem);
}


// renew
void MENU_RenewPage(Page* page) {
    // Req Condition
    if (page == NULL) {printf("%s: page not exists.\n", __func__); return;}

    //
    for (PageVolume i = 0; i < PAGE_MAX_VOLUME; i++) {
        if (page->elems[i].id != 0) {MENU_RenewElem(&page->elems[i]);}
    }
}



// renew
void MENU_RenewMenuPathString() {
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
void MENU_RenewMenuPath() {
    bool need_clear = false;
    for (PathId i = 0; i < MENU_PATH_VOLUME; i++) {
        if (need_clear) {menu.path[i] = 0;}
        else if (getMenuPageFromPathId(i) == NULL) {need_clear = true;}
    }
}
void MENU_RenewMenuPageNow() {
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

void MENU_RenewMenu() {
    static bool need_load = true;
    if (need_load || debug.on) {
        need_load = false;
    }
    MENU_RenewMenuPath();
    MENU_RenewMenuPageNow();
    MENU_RenewPage(menu.pageNow);
    // printf("%s\n", menu.pageNow->name);
    // printMenuPath();
    MENU_RenewMenuPathString();
}


#endif //RENEW_H
