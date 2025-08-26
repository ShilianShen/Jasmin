#ifndef JASMIN_MENU_INTERFACE_H
#define JASMIN_MENU_INTERFACE_H


#include "../interface.h"


#define TEMPO_MENU_JSON "../config/tempo_menu.json"
#define TEMPO_THEME_JSON "../config/tempo_theme.json"
#define TEMPO_OFEN_RELOAD 0


extern const KeyVal TEMPO_TABLE_INT[];
extern const int TEMPO_TABLE_INT_LEN;


extern const KeyVal TEMPO_TABLE_FLOAT[];
extern const int TEMPO_TABLE_FLOAT_LEN;


extern const KeyVal TEMPO_TABLE_PAGE[];


extern const KeyVal TEMPO_TABLE_BOOL[];
extern const int TEMPO_TABLE_BOOL_LEN;


extern const KeyVal TEMPO_MENU_TRIG_SET[];
extern const int TEMPO_MENU_TRIG_SET_LEN;


bool TEMPO_Init();
void TEMPO_Exit();
bool TEMPO_Renew();
bool TEMPO_Draw();


#endif //JASMIN_MENU_INTERFACE_H