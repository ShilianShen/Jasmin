#include "trig.h"


void BASIC_TrigPass(const TrigPara para) {
    return;
}
void BASIC_PullTrig(const Trig trig) {
    if (trig.func != NULL) {
        trig.func(trig.para);
    }
}
