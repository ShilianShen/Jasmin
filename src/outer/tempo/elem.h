#ifndef JASMIN_MENU_STRUCT_ELEM_H
#define JASMIN_MENU_STRUCT_ELEM_H


#include "theme.h"


typedef struct Elem Elem;
typedef struct ElemTT {
    const char *name;

} ElemTT;


bool TEMPO_SetElemDstRect(Elem *elem, SDL_FRect dst_rect);
bool TEMPO_GetElemDstRect(const Elem *elem, SDL_FRect *dst);
bool TEMPO_SetElemPublicBck(const SDL_FRect* bck);
bool TEMPO_SetElemPublicTable(const Table *table);


Elem* TEMPO_CreateElem(const cJSON *elem_json);
Elem* TEMPO_DeleteElem(Elem* elem);
bool TEMPO_RenewElem(Elem* elem);
bool TEMPO_DrawElem(const Elem* elem);


void TEMPO_TrigFuncSwitch(const char*);


#endif //JASMIN_MENU_STRUCT_ELEM_H