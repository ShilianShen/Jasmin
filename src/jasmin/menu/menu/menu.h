#ifndef JASMIN_MENU_MENU_H
#define JASMIN_MENU_MENU_H


#include "../page/page.h"

#define MENU_PAGE_VOLUME 16
#define MENU_PATH_VOLUME 6


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


void MENU_Init(SDL_Renderer* renderer);
void MENU_Deinit();
void MENU_Load(const char* menuPagesPath, const char* menuThemePath);
void MENU_Unload();
void MENU_Renew(SDL_FRect bck_rect);
void MENU_Draw();


#endif //JASMIN_MENU_MENU_H