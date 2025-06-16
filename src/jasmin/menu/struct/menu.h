#ifndef MENU_H
#define MENU_H


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


#endif //MENU_H