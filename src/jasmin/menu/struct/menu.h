#ifndef JASMIN_MENU_STRUCT_MENU_H
#define JASMIN_MENU_STRUCT_MENU_H


#include "page.h"


typedef int PathId;
const PageId MENU_PAGE_VOLUME = 16;
const PathId MENU_PATH_VOLUME = 6;
const char MENU_ROOT_NAME[] = "Root";
const char MENU_EDGE_NAME[] = "Edge";


struct {
    SDL_Renderer* renderer;  // malloc???
    SDL_FRect bck_rect;

    PageId path[MENU_PATH_VOLUME];
    char pathString[128];

    Page *pageRoot, *pageEdge, *pageNow, *pages[MENU_PAGE_VOLUME];  // malloc

    struct {
        TTF_Font* font;  // malloc
        SDL_Color color;
    } theme;
} menu;


Page* getMenuPageFromPathId(const PathId pathId) {
    return menu.pages[menu.path[pathId]];
}
Page* getMenuPageFromPageId(const PageId pageId) {
    return menu.pages[pageId];
}


#endif //JASMIN_MENU_STRUCT_MENU_H