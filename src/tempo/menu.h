#ifndef JASMIN_MENU_MENU_H
#define JASMIN_MENU_MENU_H


#include "page.h"


extern const Table TEMPO_INNER_TRIG_TABLE;
bool TEMPO_LoadMenu();
void TEMPO_UnloadMenu();


bool TEMPO_RenewMenu();
void TEMPO_DrawMenu();


#endif //JASMIN_MENU_MENU_H