#ifndef ELEM_H
#define ELEM_H


/*
 * Elem是图形显示的单位, 同时和一个TrigFunc(可以是NULL)绑定, 以实现复杂的功能.
 */

typedef uint8_t Anchor;
typedef int ElemId; // uint?
typedef char* ElemStr;
typedef enum {OUTSIDE, INSIDE, PRESSED, RELEASE, NUM_ELEM_STATES} ElemState;


typedef struct {
    // file
    ElemId id;  // id意味着page的第几个元素, 从1开始, id=0意味着未初始化
    Anchor anchor;
    SDL_FRect guide;
    TrigFunc func;
    TrigPara para; // malloc
    ElemStr string; // malloc
    SDL_Texture* texture; // malloc
    // renewable
    SDL_FRect src_rect, dst_rect;
    ElemState state;
} Elem;


#endif //ELEM_H
