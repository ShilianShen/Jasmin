#ifndef JASMIN_MENU_STRUCT_ELEM_H
#define JASMIN_MENU_STRUCT_ELEM_H


#include "../trig/trig.h"


typedef enum ElemState {OUTSIDE, INSIDE, PRESSED, RELEASE, NUM_ELEM_STATES} ElemState;


typedef struct Elem {
    // given in LOAD
    bool on;
    int id;
    int anchor;
    SDL_FRect guide;
    TrigFunc trig_func; // maybe NULL

    // malloc in LOAD
    char* trig_para; // maybe NULL
    char* string;
    SDL_Texture* texture;

    // given in RENEW
    SDL_FRect src_rect, dst_rect;
    ElemState state;
} Elem;


extern SDL_Renderer* elem_renderer;
extern TTF_Font* elem_font;
extern SDL_Color elem_color;
extern SDL_FRect elem_bck_rect;


void ELEM_Init(Elem* elem);
void ELEM_Deinit(const Elem* elem);
void ELEM_Load(Elem* elem, const toml_table_t* tomlElem, int tomlElemId);
void ELEM_Unload(Elem* elem);
void ELEM_Renew(Elem* elem);
void ELEM_Draw(const Elem* elem);


void ELEM_TurnOn(Elem* elem);
void ELEM_TurnOff(Elem* elem);
bool ELEM_IfReady(const Elem* elem);
char* ELEM_GetStateName(ElemState state);


#endif //JASMIN_MENU_STRUCT_ELEM_H