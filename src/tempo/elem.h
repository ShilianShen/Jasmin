#ifndef JASMIN_MENU_STRUCT_ELEM_H
#define JASMIN_MENU_STRUCT_ELEM_H


#include "tempo.h"
#include "type.h"


typedef struct Elem Elem;
bool TEMPO_SetElemBckNow(const SDL_FRect* bck);
bool TEMPO_SetElemTableNow(const Table *table);


void *TEMPO_CreateElem(const cJSON *elem_json);
void *TEMPO_DeleteElem(void *elem_void);
bool TEMPO_RenewElem(void* elem_void);
bool TEMPO_DrawElem(const void* elem_void);


#endif //JASMIN_MENU_STRUCT_ELEM_H