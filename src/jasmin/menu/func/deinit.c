#include "func.h"


void MENU_DeinitElem(const Elem* elem) {
    // Opt Condition
    if (elem != NULL) {}
}
void MENU_DeinitPage(const Page* page) {
    // Opt Condition
    if (page != NULL) {
        for (int i = 0; i < PAGE_ELEM_VOLUME; i++) {
            MENU_DeinitElem(&page->elems[i]);
        }
    }
}
void MENU_DeinitMenu() {
    // Opt Condition
    if (menu.pageRoot != NULL) {
        MENU_DeinitPage(menu.pageRoot);
        free(menu.pageRoot); // free pageRoot
        menu.pageRoot = NULL;
    }
    if (menu.pageEdge != NULL) {
        MENU_DeinitPage(menu.pageEdge);
        free(menu.pageEdge); // free pageEdge
        menu.pageEdge = NULL;
    }
    for (int i = 1; i < MENU_PAGE_VOLUME; i++) {
        if (menu.pages[i] != NULL) {
            MENU_DeinitPage(menu.pages[i]);
            free(menu.pages[i]); // free pages
            menu.pages[i] = NULL;
        }
    }
}


void MENU_Deinit() {
    MENU_DeinitMenu();
}