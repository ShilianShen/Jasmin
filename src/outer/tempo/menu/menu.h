#ifndef JASMIN_MENU_MENU_H
#define JASMIN_MENU_MENU_H


#include "../page/page.h"


#define MENU_PATH_VOLUME 6


struct Menu {
    // malloc in INIT
    int lenPageSet;
    Page* pageRoot;
    Page* pageEdge;
    Page** pageSet;

    // path
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