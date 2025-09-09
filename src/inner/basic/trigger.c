#include "trigger.h"


void PullTrig(const Trig* trig) {
    if (trig != NULL && trig->func != NULL) {
        trig->func(trig->para);
    }
}


// CREATE & DELETE =====================================================================================================
static bool BASIC_CreateTrig_RK(Trig* trig, const TrigFunc func, const char* para, const bool sustain) {
    trig->func = func;
    trig->para = NULL;
    if (para != NULL) {
        trig->para = strdup(para);
        if (trig->para == NULL) {
            printf("%s: fail\n", __func__);
            return false;
        }
    }
    trig->sustain = sustain;
    return true;
}
Trig* BASIC_CreateTrig(const TrigFunc func, const char* para, const bool sustain) {
    if (func == NULL) {
        return NULL;
    } // Req Condition
    Trig* trig = malloc(sizeof(Trig));
    if (trig == NULL) {
        printf("%s: malloc failed\n", __func__);
        return NULL;
    } // Req Condition
    if (BASIC_CreateTrig_RK(trig, func, para, sustain) == false) {
        printf("%s: fail\n", __func__);
        trig = BASIC_DeleteTrig(trig);
    }
    return trig;
}
Trig* BASIC_DeleteTrig(Trig *trig) {
    if (trig != NULL) {
        if (trig->func != NULL) {
            trig->func = NULL;
        }
        if (trig->para != NULL) {
            free(trig->para);
            trig->para = NULL;
        }
        trig = NULL;
    }
    return trig;
}