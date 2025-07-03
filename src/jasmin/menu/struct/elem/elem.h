#ifndef JASMIN_MENU_STRUCT_ELEM_H
#define JASMIN_MENU_STRUCT_ELEM_H


#include "../trig/trig.h"


typedef uint8_t Anchor;
typedef int ElemId;
typedef char* ElemStr;
typedef enum ElemState {OUTSIDE, INSIDE, PRESSED, RELEASE, NUM_ELEM_STATES} ElemState;
typedef struct Elem {
    // file
    ElemId id;  // id意味着page的第几个元素, 从1开始, id=0意味着未初始化
    Anchor anchor;
    SDL_FRect guide;
    TrigFunc trig_func;
    TrigPara trig_para; // malloc
    ElemStr string; // malloc
    SDL_Texture* texture; // malloc
    // renewable
    SDL_FRect src_rect, dst_rect;
    ElemState state;
} Elem;


bool testElem(const Elem* elem, const char* string);
char* getElemStateName(ElemState state);


#endif //JASMIN_MENU_STRUCT_ELEM_H
