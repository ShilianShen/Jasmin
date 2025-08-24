#ifndef JASMIN_MENU_STRUCT_ELEM_H
#define JASMIN_MENU_STRUCT_ELEM_H


#include "theme.h"


typedef struct Elem Elem;


Elem* TEMPO_CreateElem(const cJSON *elem_json);
Elem* TEMPO_DeleteElem(Elem* elem);
bool TEMPO_RenewElem(Elem* elem);
bool TEMPO_DrawElem(const Elem* elem);


void TEMPO_TrigFuncSwitch(const char*);


#endif //JASMIN_MENU_STRUCT_ELEM_H