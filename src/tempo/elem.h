#ifndef JASMIN_MENU_STRUCT_ELEM_H
#define JASMIN_MENU_STRUCT_ELEM_H


#include "tempo.h"


typedef struct Elem Elem;


// bool TEMPO_SetElemDstRect(Elem *elem, SDL_FRect dst_rect);
// bool TEMPO_GetElemDstRect(const Elem *elem, SDL_FRect *dst);
bool TEMPO_SetElemPublicBck(const SDL_FRect* bck);
bool TEMPO_SetElemPublicTable(const Table *table);


void *TEMPO_CreateElem(const cJSON *elem_json);
void *TEMPO_DeleteElem(void *elem_void);
bool TEMPO_RenewElem(void* elem_void);
bool TEMPO_DrawElem(const void* elem_void);


void TEMPO_TrigFuncBool(TrigPara para);
void TEMPO_TrigFuncSlid(TrigPara para);


#endif //JASMIN_MENU_STRUCT_ELEM_H