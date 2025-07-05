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
    Page* pageRoot;
    Page* pageEdge;
    Page* pages[MENU_PAGE_VOLUME];

    // other
    SDL_FRect bck_rect;
    int path[MENU_PATH_VOLUME];
    Page* pageNow;

    //
    struct MenuTheme {
        TTF_Font* font; // malloc in LOAD
        SDL_Color color;
    } theme;
} Menu;
extern Menu menu;


Page* MENU_getPageFromPathId(int pathId);
Page* MENU_GetPageFromPageId(int pageId);


#endif //JASMIN_MENU_STRUCT_MENU_H