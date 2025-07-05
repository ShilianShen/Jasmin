#include "func.h"


void ELEM_Deinit(const Elem* elem) {
    // Opt Condition
    if (elem != NULL) {}
}
void PAGE_Deinit(const Page* page) {
    // Opt Condition
    if (page != NULL) {
        for (int i = 0; i < PAGE_ELEM_VOLUME; i++) {
            ELEM_Deinit(&page->elems[i]);
        }
    }
}
void MENU_DeinitMenu() {
    // Opt Condition
    if (menu.pageRoot != NULL) {
        PAGE_Deinit(menu.pageRoot);
        free(menu.pageRoot); // free pageRoot
        menu.pageRoot = NULL;
    }
    if (menu.pageEdge != NULL) {
        PAGE_Deinit(menu.pageEdge);
        free(menu.pageEdge); // free pageEdge
        menu.pageEdge = NULL;
    }
    for (int i = 1; i < MENU_PAGE_VOLUME; i++) {
        if (menu.pages[i] != NULL) {
            PAGE_Deinit(menu.pages[i]);
            free(menu.pages[i]); // free pages
            menu.pages[i] = NULL;
        }
    }
}


void MENU_Deinit() {
    MENU_DeinitMenu();
}