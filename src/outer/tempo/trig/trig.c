#include "../trig/trig.h"


KeyVal TEMPO_MENU_TRIG_SET[] = {
    {"pass", TEMPO_TrigFuncPass},
    {"forward", TEMPO_TrigFuncForward},
    {"backward", TEMPO_TrigFuncBackward},
    {"clear", TEMPO_TrigFuncClear},
    {"switch", TEMPO_TrigFuncSwitch},
    {NULL, NULL}
};