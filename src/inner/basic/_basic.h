#ifndef JASMIN_BASIC_INTERFACE_H
#define JASMIN_BASIC_INTERFACE_H


#include "../_inner.h"


typedef enum JSM_DATA_TYPE {
    JSM_INT,
    JSM_FLOAT,
    JSM_STRING,
    JSM_BOOL,
    JSM_RECT,
    JSM_COLOR,
    JSM_NUM_TYPES
} JSM_DATA_TYPE;
extern SDL_FRect windowRect;


bool BASIC_Init();
bool BASIC_Renew();
void BASIC_Exit();



#endif //JASMIN_BASIC_INTERFACE_H