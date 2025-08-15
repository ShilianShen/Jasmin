#ifndef JASMIN_MENU_STRUCT_ELEM_H
#define JASMIN_MENU_STRUCT_ELEM_H


#include "../trig/trig.h"


typedef struct Elem Elem;
typedef enum ElemState ElemState;
typedef enum ElemType ElemType;
typedef union ElemInfo ElemInfo;


bool TEMPO_SetElemDstRect(Elem *elem, SDL_FRect dst_rect);
bool TEMPO_GetElemDstRect(const Elem *elem, SDL_FRect *dst_rect);


Elem* TEMPO_CreateElem(const toml_table_t *tomlElem);
Elem* TEMPO_DeleteElem(Elem* elem);
bool TEMPO_RenewElem(Elem* elem);
bool TEMPO_DrawElem(const Elem* elem);


#endif //JASMIN_MENU_STRUCT_ELEM_H