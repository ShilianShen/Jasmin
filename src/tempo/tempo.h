#ifndef JASMIN_MENU_INTERFACE_H
#define JASMIN_MENU_INTERFACE_H


#include "../perph/perph.h"
#define TEMPO_DEFAULT_MENU_JSON "../config/tempo_menu.json"
#define TEMPO_DEFAULT_THEME_JSON "../config/tempo_theme.json"


extern const Table TEMPO_OUTER_TRIG_FUNC_TABLE;
extern const Table TEMPO_ExternTable[];
extern Table TEMPO_ExternPageTable;


bool TEMPO_Init();
void TEMPO_Exit();
bool TEMPO_Renew();
bool TEMPO_Draw();


#endif //JASMIN_MENU_INTERFACE_H