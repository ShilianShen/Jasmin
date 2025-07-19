#ifndef JASMIN_MENU_STRUCT_ELEM_H
#define JASMIN_MENU_STRUCT_ELEM_H


#include "../trig/trig.h"


typedef enum ElemState ElemState;
typedef enum ElemType ElemStrType;
typedef struct Elem Elem;


extern SDL_Renderer* elem_renderer;
extern TTF_Font* elem_font;
extern SDL_Color elem_color;
extern SDL_FRect elem_bck_rect;



void ELEM_SetGidRect(Elem* elem, SDL_FRect gid_rect);


void ELEM_RenewOk(Elem* elem);
Elem* ELEM_Malloc(const toml_table_t* tomlElem, int tomlElemId);
void ELEM_Free(Elem* elem);
void ELEM_Renew(Elem* elem);
void ELEM_Draw(const Elem* elem);


bool ELEM_IfOk(const Elem* elem);


#endif //JASMIN_MENU_STRUCT_ELEM_H