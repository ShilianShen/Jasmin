#include "trigger.h"


void PullTrig(const Trig* trig) {
    if (trig != NULL && trig->func != NULL) {
        trig->func(trig->para);
    }
}


// CREATE & DELETE =====================================================================================================
Trig* BASIC_CreateTrig(const TrigFunc func, const void *para, const bool sustain) {
    if (func == NULL) {
        return NULL;
    } // Req Condition
    Trig* trig = malloc(sizeof(Trig));
    if (trig == NULL) {
        printf("%s: malloc failed\n", __func__);
        return NULL;
    } // Req Condition

    trig->func = func;
    trig->para = para;
    trig->sustain = sustain;

    return trig;
}
Trig* BASIC_DeleteTrig(Trig *trig) {
    if (trig != NULL) {
        if (trig->func != NULL) {
            trig->func = NULL;
        }
        if (trig->para != NULL) {
            trig->para = NULL;
        }
        trig = NULL;
    }
    return trig;
}