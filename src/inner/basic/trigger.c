#include "trigger.h"


void PullTrig(const Trig* trig) {
    if (trig != NULL && trig->func != NULL) {
        trig->func(trig->para);
    }
}


bool CreateTrig_RK(Trig* trig, const TrigFunc func, const char* para) {
    trig->func = func;
    trig->para = NULL;
    if (para != NULL) {
        trig->para = strdup(para);
        if (trig->para == NULL) {
            printf("%s: fail\n", __func__);
            return false;
        }
    }
    return true;
}
Trig* CreateTrig(const TrigFunc func, const char* para) {
    if (func == NULL) {
        return NULL;
    } // Req Condition
    Trig* trig = malloc(sizeof(Trig));
    if (trig == NULL) {
        printf("%s: malloc failed\n", __func__);
        return NULL;
    } // Req Condition
    if (CreateTrig_RK(trig, func, para) == false) {
        printf("%s: fail\n", __func__);
        trig = DeleteTrig(trig);
    }
    return trig;
}
Trig* DeleteTrig(Trig *trig) {
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
