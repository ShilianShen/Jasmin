#include "func.h"


static void MENU_DeinitMenu() {
    // Opt Condition
    if (menu.pageRoot != NULL) {
        free(menu.pageRoot);
        menu.pageRoot = NULL;
    }
    if (menu.pageEdge != NULL) {
        free(menu.pageEdge);
        menu.pageEdge = NULL;
    }
    for (int i = 1; i < MENU_PAGE_VOLUME; i++) {
        if (menu.pages[i] != NULL) {
            free(menu.pages[i]);
            menu.pages[i] = NULL;
        }
    }
}
void MENU_Deinit() {
    MENU_DeinitMenu();
}