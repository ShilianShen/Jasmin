#ifndef I_ENG_H
#define I_ENG_H


#include "../basic/basic.h"


typedef struct {
    int subject;
    int action;
    int object;
} intelligence;


bool I_ENG_Init();
bool I_ENG_Renew();
bool I_ENG_Draw();
void I_ENG_Exit();


#endif //I_ENG_H