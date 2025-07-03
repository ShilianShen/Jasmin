#include "func.h"


static void MENU_UnloadElem(Elem* elem) {
    // Req Condition
    if (elem == NULL) {printf("%s: elem not exists.\n", __func__); return;}

    //
    if (elem->string != NULL) {free(elem->string); elem->string = NULL;}
    if (elem->trig_para != NULL) {free(elem->trig_para); elem->trig_para = NULL;}
    if (elem->texture != NULL) {SDL_DestroyTexture(elem->texture); elem->texture = NULL;}
}
static void MENU_UnloadPage(Page* page) {
    // Req Condition
    if (page == NULL) {printf("%s: page not exists.\n", __func__); return;}

    //
    if (page->name != NULL) {free(page->name); page->name = NULL;}
    for (int i = 0; i < PAGE_MAX_VOLUME; i++) {MENU_UnloadElem(&page->elems[i]);}
}
static void MENU_UnloadMenuTheme() {
    if (menu.theme.font != NULL) {
        TTF_CloseFont(menu.theme.font);
        menu.theme.font = NULL;
    }
}
static void MENU_UnloadMenu() {
    MENU_UnloadPage(menu.pageRoot);
    MENU_UnloadPage(menu.pageEdge);
    for (int i = 0; i < MENU_PAGE_VOLUME; i++) {
        if (menu.pages[i] != NULL) {
            MENU_UnloadPage(menu.pages[i]);
        }
    }
    MENU_UnloadMenuTheme();
}


void MENU_Unload() {
    MENU_UnloadMenu();
}