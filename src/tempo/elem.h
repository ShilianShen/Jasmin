#ifndef JASMIN_MENU_STRUCT_ELEM_H
#define JASMIN_MENU_STRUCT_ELEM_H


#include "tempo.h"
#include "type.h"


extern bool trigAuthorize;
typedef struct Elem Elem;
bool TEMPO_SetElemBckNow(const SDL_FRect* bck);
bool TEMPO_SetElemTableNow(const Table *table);


Elem *TEMPO_CreateElem(const cJSON *elem_json);
Elem *TEMPO_DeleteElem(Elem *elem);
bool TEMPO_RenewElem(Elem *elem);
bool TEMPO_DrawElem(const Elem *elem);


#endif //JASMIN_MENU_STRUCT_ELEM_H