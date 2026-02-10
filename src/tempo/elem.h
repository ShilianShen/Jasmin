#ifndef JASMIN_MENU_STRUCT_ELEM_H
#define JASMIN_MENU_STRUCT_ELEM_H


#include "tempo.h"
#include "type.h"


extern bool trigAuthorize;
typedef struct TEMPO_Elem TEMPO_Elem;
bool TEMPO_SetElemBckNow(const SDL_FRect* bck);
bool TEMPO_SetElemTableNow(const Table *table);


TEMPO_Elem *TEMPO_CreateElem(const cJSON *elem_json);
TEMPO_Elem *TEMPO_DeleteElem(TEMPO_Elem *elem);
bool TEMPO_RenewElem(TEMPO_Elem *elem);
bool TEMPO_DrawElem(const TEMPO_Elem *elem);


#endif //JASMIN_MENU_STRUCT_ELEM_H