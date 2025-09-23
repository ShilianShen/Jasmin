#ifndef TRIG_H
#define TRIG_H


#include "_basic.h"


typedef void (*TrigFunc)(const void*);
struct Trig {
    TrigFunc func;
    const void* para;
    bool sustain;
};
typedef struct Trig Trig;



void PullTrig(const Trig* trig);
Trig* BASIC_CreateTrig(TrigFunc func, const void *para, bool sustain); // fuckit
Trig* BASIC_DeleteTrig(Trig *trig);


#endif //TRIG_H