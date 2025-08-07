#ifndef JASMIN_MENU_MENU_H
#define JASMIN_MENU_MENU_H


#include "../page/page.h"


#define MENU_PAGE_VOLUME 16
#define MENU_PATH_VOLUME 6


struct Menu {
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
};
typedef struct Menu Menu;
extern Menu menu;


void TEMPO_InitMenu();
void TEMPO_DeinitMenu();
void TEMPO_LoadMenu();
void TEMPO_UnloadMenu();
void TEMPO_RenewMenu();
void TEMPO_DrawMenu();


#endif //JASMIN_MENU_MENU_H