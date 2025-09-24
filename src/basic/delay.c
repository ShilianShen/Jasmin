#include "delay.h"


bool BASIC_SetDelay(Delay* delay, const float time) {
    REQ_CONDITION(delay != NULL, return false);

    const float t = (float)SDL_GetTicks() / 1000;
    if (delay->block == true && t < delay->t2) return false;

    delay->t1 = t;
    delay->t2 = t + time;
    return true;
}
bool BASIC_GetDelay(const Delay* delay, float* value) {
    REQ_CONDITION(delay != NULL && value != NULL, return false);

    if (delay->t1 == delay->t2) return 1;

    const float t = (float)SDL_GetTicks() / 1000;
    *value = (t - delay->t1) / (delay->t2 - delay->t1);
    return true;
}