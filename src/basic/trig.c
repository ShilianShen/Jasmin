#include "trig.h"


static void BASIC_TrigFuncPass(TrigPara para) {
    (void* )(para);
    return;
}
Trig BASIC_TrigPass = {BASIC_TrigFuncPass, 0, false};
void BASIC_PullTrig(const Trig trig) {
    if (trig.func != NULL) {
        trig.func(trig.para);
    }
}
