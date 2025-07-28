#ifndef JASMIN_MENU_STRUCT_ELEM_H
#define JASMIN_MENU_STRUCT_ELEM_H


#include "../trig/trig.h"


typedef struct Elem Elem;
typedef enum ElemState ElemState;
typedef enum ElemType ElemType;
typedef struct ElemInfo ElemInfo;


enum ElemType {
    ELEM_TYPE_DEFAULT,
    ELEM_TYPE_FILE,
    ELEM_TYPE_TEXT,
    ELEM_NUM_TYPES
};
struct ElemInfo {
    ElemType type;
    const char* string;
    int anchor;
    const SDL_FRect* gid_rect;
    TrigFunc trig_func;
    const char* trig_para;
};


extern const SDL_FRect ELEM_DEFAULT_GID_RECT;


void TEMPO_SetElemGidRect(Elem* elem, SDL_FRect gid_rect);
void TEMPO_SetElemDstRect(Elem* elem, SDL_FRect dst_rect);
void TEMPO_GetElemDstRect(const Elem* elem, SDL_FRect* dst_rect);
bool TEMPO_GetElemOk(const Elem* elem);


Elem* TEMPO_CreateElem(ElemInfo info);
Elem* TEMPO_CreateElemFromToml(const toml_table_t*);
void TEMPO_DestroyElem(Elem* elem);
void TEMPO_RenewElem(Elem* elem);
void TEMPO_DrawElem(Elem *elem);


#endif //JASMIN_MENU_STRUCT_ELEM_H