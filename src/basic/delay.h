#ifndef DELAY_H
#define DELAY_H


#include "_basic.h"
#include "atv.h"


typedef struct {
    float t1, t2;
    bool block;
} Delay;


bool BASIC_SetDelay(Delay* delay, float time);
bool BASIC_GetDelay(const Delay* delay, float* value);


#endif //DELAY_H