#include "trigger.h"


void PullTrig(const Trig* trig) {
    if (trig != NULL && trig->func != NULL) {
        trig->func(trig->para);
    }
}