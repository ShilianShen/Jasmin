#ifndef JASMIN_MENU_STRUCT_MENU_H
#define JASMIN_MENU_STRUCT_MENU_H


#include "../page/page.h"


#define MENU_PAGE_VOLUME 16
#define MENU_PATH_VOLUME 6
extern const char MENU_ROOT_NAME[];
extern const char MENU_EDGE_NAME[];


typedef struct Menu {
    // given in INIT
    SDL_Renderer* renderer;

    // malloc in INIT
    Page* pageRoot;                // pageId = 0
    Page* pageEdge;                // pageId = MENU_PAGE_VOLUME
    Page* pages[MENU_PAGE_VOLUME]; // 0 < pageId < MENU_PAGE_VOLUME, pages[0] = NULL

    // other
    SDL_FRect bck_rect;
    PageId path[MENU_PATH_VOLUME];
    Page* pageNow;

    //
    struct MenuTheme {
        TTF_Font* font; // malloc in LOAD
        SDL_Color color;
    } theme;
} Menu;
extern Menu menu;


Page* getMenuPageFromPathId(int pathId);
Page* getMenuPageFromPageId(int pageId);


#endif //JASMIN_MENU_STRUCT_MENU_H