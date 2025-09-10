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



void PullTrig(const Trig* trig);
Trig* BASIC_CreateTrig(TrigFunc func, void *para, bool sustain);
Trig* BASIC_DeleteTrig(Trig *trig);


#endif //TRIG_H