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


// extern ma_result result;
// extern ma_engine engine;

bool BASIC_Init();
bool BASIC_Renew();
void BASIC_Exit();


void MA_PlaySound(const char *sound);



#endif //JASMIN_BASIC_INTERFACE_H