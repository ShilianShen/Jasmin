#ifndef JASMIN_MENU_INTERFACE_H
#define JASMIN_MENU_INTERFACE_H


#include "../interface.h"
#define TEMPO_DEFAULT_MENU_JSON "../config/tempo_menu.json"
#define TEMPO_DEFAULT_THEME_JSON "../config/tempo_theme.json"
#define TEMPO_OFEN_RELOAD 0


extern const Table TEMPO_ExternTable[];
extern const Table TEMPO_StaticTrigTable;


bool TEMPO_Init();
void TEMPO_Exit();
bool TEMPO_Renew();
bool TEMPO_Draw();


#endif //JASMIN_MENU_INTERFACE_H