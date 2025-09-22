#ifndef VILLA_H
#define VILLA_H


#include "../device/device.h"
#include "../tempo/tempo.h"
#include "../lotri/lotri.h"


typedef enum VILLA_DATA_TYPE VILLA_DATA_TYPE;
enum VILLA_DATA_TYPE {
    VILLA_DATA_NONE,
    VILLA_DATA_CHARACTER,
    VILLA_DATA_ROOM,
    VILLA_DATA_WALL,
    VILLA_NUM_DATA_TYPES
};


bool VILLA_Init();
bool VILLA_Renew();
bool VILLA_Draw();
void VILLA_Exit();


#endif //VILLA_H