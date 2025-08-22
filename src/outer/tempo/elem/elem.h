#ifndef JASMIN_MENU_STRUCT_ELEM_H
#define JASMIN_MENU_STRUCT_ELEM_H


#include "../basic/basic.h"


typedef struct Elem Elem;


Elem* TEMPO_CreateElem(const toml_table_t *tomlElem);
Elem* TEMPO_DeleteElem(Elem* elem);
bool TEMPO_RenewElem(Elem* elem);
bool TEMPO_DrawElem(const Elem* elem);


#endif //JASMIN_MENU_STRUCT_ELEM_H