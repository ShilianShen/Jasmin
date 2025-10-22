#include "trig.h"


void BASIC_PullTrig(const Trig trig) {
    if (trig.func != NULL) {
        trig.func(trig.para);
    }
}
