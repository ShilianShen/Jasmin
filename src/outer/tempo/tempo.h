#ifndef JASMIN_MENU_INTERFACE_H
#define JASMIN_MENU_INTERFACE_H


#include "../interface.h"


#define TEMPO_MENU_JSON "../config/tempo_menu.json"
#define TEMPO_THEME_JSON "../config/tempo_theme.json"
#define TEMPO_OFEN_RELOAD 0


// extern KeyVal TEMPO_INT[];
// extern KeyVal TEMPO_FLOAT[];
// extern KeyVal TEMPO_BOOL[];
extern const Table TEMPO_ExternTable[];



extern const KeyVal TEMPO_MENU_TRIG_SET[];
extern const int TEMPO_MENU_TRIG_SET_LEN;


bool TEMPO_Init();
void TEMPO_Exit();
bool TEMPO_Renew();
bool TEMPO_Draw();


#endif //JASMIN_MENU_INTERFACE_H