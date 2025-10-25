#ifndef TRIG_H
#define TRIG_H


#include "_basic.h"


typedef intptr_t TrigPara;
typedef void (*TrigFunc)(TrigPara);
struct Trig {
    TrigFunc func;
    TrigPara para;
    bool sustain;
};
typedef struct Trig Trig;


void BASIC_PullTrig(Trig trig);


#endif //TRIG_H