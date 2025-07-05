#include "func.h"


void ELEM_RenewDstRect(Elem* elem) {
    // Req Condition
    if (ELEM_IfReady(elem) == false) {
        printf("%s: elem not ready.\n", __func__);
        return;
    }

    //
    SDL_LoadDstRectAligned(&elem->dst_rect, elem->texture, &elem->src_rect, &elem->guide, &menu.bck_rect, elem->anchor);
}
void ELEM_RenewState(Elem* elem) {
    // Req Condition
    if (!ELEM_IfReady(elem)) {
        printf("%s: elem not ready.\n", __func__);
        return;
    }

    //
    const bool mouseIn = mouseInRect(&elem->dst_rect);
    const bool mouseLeftIn = mouseLeftInRect(&elem->dst_rect);
    if (elem->state == PRESSED) {
        DEBUG_SendMessageL("elem.on: %d\n", elem->on);
        DEBUG_SendMessageL("elem.id: %d\n", elem->id);
        DEBUG_SendMessageL("elem.state: %s\n", ELEM_GetStateName(elem->state));
        if (elem->trig_func != NULL) {
            DEBUG_SendMessageL("elem.trig: %s, %s\n", TRIG_FindNameFromFunc(elem->trig_func), elem->trig_para);
        }
    }
    if (elem->state == PRESSED && mouseIn == true && mouseLeftIn == false) {
        elem->state = RELEASE;
    }
    else {
        if (mouseIn == true) {
            elem->state = mouseLeftIn ? PRESSED : INSIDE;
        }
        else {
            elem->state = OUTSIDE;
        }
    }
    if (elem->state == RELEASE && elem->trig_func != NULL) {
        elem->trig_func(elem->trig_para);
    }
}
void MENU_RenewElem(Elem* elem) {
    // Req Condition
    if (!ELEM_IfReady(elem)) {
        printf("%s: elem not ready.\n", __func__);
        return;
    }

    //
    ELEM_RenewDstRect(elem);
    ELEM_RenewState(elem);
}


void MENU_RenewPage(Page* page) {
    // Req Condition
    if (page == NULL) {
        printf("%s: page not exists.\n", __func__);
        return;
    }

    //
    for (int i = 0; i < PAGE_ELEM_VOLUME; i++) {
        if (page->elems[i].on == true) {
            MENU_RenewElem(&page->elems[i]);
        }
    }
}

void MENU_RenewMenuPath() {
    bool need_clear = false;
    DEBUG_SendMessageL("menu.path: /%s", menu.pageRoot->name);
    for (int i = 0; i < MENU_PATH_VOLUME; i++) {
        if (need_clear) {
            menu.path[i] = 0;
        }
        else if (MENU_getPageFromPathId(i) == NULL) {
            need_clear = true;
        }
        else {
            DEBUG_SendMessageL("/%s", MENU_getPageFromPathId(i)->name);
        }
    }
    if (need_clear == false) {
        DEBUG_SendMessageL("/%s", menu.pageEdge->name);
    }
    DEBUG_SendMessageL("\n");
}
void MENU_RenewMenuPageNow() {
    // if pageRoot
    if (MENU_getPageFromPathId(0) == NULL) {
        menu.pageNow = menu.pageRoot;
        return;
    }
    // else if pageOther
    for (int i = 0; i + 1 < MENU_PATH_VOLUME; i++) {
        if (MENU_getPageFromPathId(i) != NULL && MENU_getPageFromPathId(i+1) == NULL) {
            menu.pageNow = MENU_getPageFromPathId(i);
            return;
        }
    }
    // else pageEdge
    menu.pageNow = menu.pageEdge;
}
void MENU_RenewMenu() {
    static bool need_load = true;
    MENU_RenewMenuPath();
    MENU_RenewMenuPageNow();
    MENU_RenewPage(menu.pageNow);
    // printf("%s\n", menu.pageNow->name);
    // printMenuPath();
}


void MENU_Renew() {
    MENU_RenewMenu();
}