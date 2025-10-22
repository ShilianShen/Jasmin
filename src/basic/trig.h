#ifndef TRIG_H
#define TRIG_H


#include "_basic.h"


typedef void (*TrigFunc)(void*);
struct Trig {
    TrigFunc func;
    void* para;
    bool sustain;
};
typedef struct Trig Trig;


void BASIC_PullTrig(Trig trig);


#endif //TRIG_H