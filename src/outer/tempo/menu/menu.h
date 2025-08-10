#ifndef JASMIN_MENU_MENU_H
#define JASMIN_MENU_MENU_H


#include "../page/page.h"


#define LEN_PAGE_SET 16
#define MENU_PATH_VOLUME 6


struct Menu {
    // malloc in INIT
    int lenPageSet;
    Page* pageRoot;
    Page* pageEdge;
    Page* pageSet[LEN_PAGE_SET];
    const char* pageNameSet[LEN_PAGE_SET];

    // other
    SDL_FRect bck_rect;
    Page* path[MENU_PATH_VOLUME];
    Page* pageNow;
};
typedef struct Menu Menu;
extern Menu menu;


void TEMPO_LoadMenu();
void TEMPO_UnloadMenu();
void TEMPO_RenewMenu();
void TEMPO_DrawMenu();


#endif //JASMIN_MENU_MENU_H