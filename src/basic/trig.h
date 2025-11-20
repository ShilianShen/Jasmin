#ifndef TRIG_H
#define TRIG_H


#include "_basic.h"


typedef intptr_t TrigPara;
typedef void (*TrigFunc)(TrigPara);
typedef struct {
    TrigFunc func;
    TrigPara para;
    bool sustain;
} Trig;


void BASIC_TrigPass(TrigPara para);
void BASIC_PullTrig(Trig trig);


#endif //TRIG_H