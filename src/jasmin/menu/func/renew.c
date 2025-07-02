#include "func.h"


static void MENU_RenewElemDstRect(Elem* elem) {
    // Req Condition
    if (!testElem(elem, __func__)) {return;}

    //
    SDL_LoadDstRectAligned(&elem->dst_rect, elem->texture, &elem->src_rect, &elem->guide, &menu.bck_rect, elem->anchor);
}
static void MENU_RenewElemState(Elem* elem) {
    // Req Condition
    if (!testElem(elem, __func__)) {return;}

    //
    const bool mouseIn = mouseInRect(&elem->dst_rect);
    const bool mouseLeftIn = mouseLeftInRect(&elem->dst_rect);
    if (elem->state == PRESSED) {
        DEBUG_SendMessage("Elem: %d\n", elem->id);
        DEBUG_SendMessage("State: %d\n", elem->state);
        DEBUG_SendMessage("Trig: %s, %s\n", TRIG_FindNameFromFunc(elem->trig_func), elem->trig_para);
    }
    if (elem->state == PRESSED && mouseIn == true && mouseLeftIn == false) {elem->state = RELEASE;}
    else {
        if (mouseIn == true) {elem->state = mouseLeftIn ? PRESSED : INSIDE;}
        else {elem->state = OUTSIDE;}
    }
    if (elem->state == RELEASE && elem->trig_func != NULL) {elem->trig_func(elem->trig_para);}
}
static void MENU_RenewElem(Elem* elem) {
    // Req Condition
    if (!testElem(elem, __func__)) {return;}

    //
    MENU_RenewElemDstRect(elem);
    MENU_RenewElemState(elem);
}


static void MENU_RenewPage(Page* page) {
    // Req Condition
    if (page == NULL) {printf("%s: page not exists.\n", __func__); return;}

    //
    for (PageVolume i = 0; i < PAGE_MAX_VOLUME; i++) {
        if (page->elems[i].id != 0) {MENU_RenewElem(&page->elems[i]);}
    }
}


static void MENU_RenewMenuPathString() {
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
static void MENU_RenewMenuPath() {
    bool need_clear = false;
    for (PathId i = 0; i < MENU_PATH_VOLUME; i++) {
        if (need_clear) {menu.path[i] = 0;}
        else if (getMenuPageFromPathId(i) == NULL) {need_clear = true;}
    }
}
static void MENU_RenewMenuPageNow() {
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
static void MENU_RenewMenu() {
    static bool need_load = true;
    MENU_RenewMenuPath();
    MENU_RenewMenuPageNow();
    MENU_RenewPage(menu.pageNow);
    // printf("%s\n", menu.pageNow->name);
    // printMenuPath();
    MENU_RenewMenuPathString();
}


void MENU_Renew() {
    MENU_RenewMenu();
}