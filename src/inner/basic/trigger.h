#ifndef TRIG_H
#define TRIG_H

#include "setting.h"
typedef void (*TrigFunc)(const char*);
struct Trig {
    TrigFunc func;
    char* para;
};
typedef struct Trig Trig;



void PullTrig(const Trig* trig);
Trig* CreateTrig(TrigFunc func, const char* para);
Trig* DeleteTrig(Trig *trig);


#endif //TRIG_H