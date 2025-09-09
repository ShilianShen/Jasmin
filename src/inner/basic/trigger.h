#ifndef TRIG_H
#define TRIG_H


#include "_basic.h"


typedef void (*TrigFunc)(const char*);
struct Trig {
    bool sustain;
    TrigFunc func;
    char* para;
};
typedef struct Trig Trig;



void PullTrig(const Trig* trig);
Trig* BASIC_CreateTrig(TrigFunc func, const char* para, bool sustain);
Trig* BASIC_DeleteTrig(Trig *trig);


#endif //TRIG_H