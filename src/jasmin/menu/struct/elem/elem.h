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


void ELEM_TurnOn(Elem* elem);
void ELEM_TurnOff(Elem* elem);
bool ELEM_IfReady(const Elem* elem);
char* ELEM_GetStateName(ElemState state);


#endif //JASMIN_MENU_STRUCT_ELEM_H