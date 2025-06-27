#ifndef JASMIN_MENU_STRUCT_MENU_H
#define JASMIN_MENU_STRUCT_MENU_H


#include "../page/page.h"


typedef int PathId;


#define MENU_PAGE_VOLUME 16
#define MENU_PATH_VOLUME 6
extern const char MENU_ROOT_NAME[];
extern const char MENU_EDGE_NAME[];


typedef struct Menu  {
    SDL_Renderer* renderer;  // malloc???
    SDL_FRect bck_rect;

    PageId path[MENU_PATH_VOLUME];
    char pathString[128];

    Page *pageRoot, *pageEdge, *pageNow, *pages[MENU_PAGE_VOLUME];  // malloc

    struct {
        TTF_Font* font;  // malloc
        SDL_Color color;
    } theme;
} Menu;
extern Menu menu;


Page* getMenuPageFromPathId(const PathId pathId);
Page* getMenuPageFromPageId(const PageId pageId);


#endif //JASMIN_MENU_STRUCT_MENU_H