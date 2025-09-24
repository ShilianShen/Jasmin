#ifndef JASMIN_MENU_STRUCT_ELEM_H
#define JASMIN_MENU_STRUCT_ELEM_H


#include "theme.h"


typedef struct Elem Elem;


bool TEMPO_SetElemDstRect(Elem *elem, SDL_FRect dst_rect);
bool TEMPO_GetElemDstRect(const Elem *elem, SDL_FRect *dst);
bool TEMPO_SetElemPublicBck(const SDL_FRect* bck);
bool TEMPO_SetElemPublicTable(const Table *table);


void *TEMPO_CreateElem(const cJSON *elem_json);
void *TEMPO_DeleteElem(void *elem_void);
bool TEMPO_RenewElem(Elem* elem);
bool TEMPO_DrawElem(const Elem* elem);


void TEMPO_TrigFuncBool(const void *para);
void TEMPO_TrigFuncSlid(const void *para);


#endif //JASMIN_MENU_STRUCT_ELEM_H