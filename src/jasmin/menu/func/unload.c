#include "func.h"


void ELEM_Unload(Elem* elem) {
    // Req Condition
    if (elem == NULL) {printf("%s: elem not exists.\n", __func__); return;}

    //
    if (elem->string != NULL) {free(elem->string); elem->string = NULL;}
    if (elem->trig_para != NULL) {free(elem->trig_para); elem->trig_para = NULL;}
    if (elem->texture != NULL) {SDL_DestroyTexture(elem->texture); elem->texture = NULL;}
    ELEM_TurnOff(elem);
}
void PAGE_Unload(Page* page) {
    // Req Condition
    if (page == NULL) {printf("%s: page not exists.\n", __func__); return;}

    //
    if (page->name != NULL) {free(page->name); page->name = NULL;}
    for (int i = 0; i < PAGE_ELEM_VOLUME; i++) {ELEM_Unload(&page->elems[i]);}
}
void MENU_UnloadMenuTheme() {
    if (menu.theme.font != NULL) {
        TTF_CloseFont(menu.theme.font);
        menu.theme.font = NULL;
    }
}
void MENU_UnloadMenu() {
    PAGE_Unload(menu.pageRoot);
    PAGE_Unload(menu.pageEdge);
    for (int i = 0; i < MENU_PAGE_VOLUME; i++) {
        if (menu.pages[i] != NULL) {
            PAGE_Unload(menu.pages[i]);
        }
    }
    MENU_UnloadMenuTheme();
}


void MENU_Unload() {
    MENU_UnloadMenu();
}