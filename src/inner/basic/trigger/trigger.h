#ifndef TRIG_H
#define TRIG_H

#include "../setting/setting.h"
typedef void (*TrigFunc)(char*);
struct Trig {
    TrigFunc func;
    char* para;
};
typedef struct Trig Trig;



void PullTrig(const Trig* trig);


#endif //TRIG_H