#ifndef LOTRI_BUFFER_H
#define LOTRI_BUFFER_H


#include "model.h"
#include "world.h"
#include "mw.h"


bool LOTRI_ClearBuffer();
bool LOTRI_SendBuffer(LOTRI_World* world);
bool LOTRI_RenewBuffer();
bool LOTRI_DrawBuffer();


#endif //LOTRI_BUFFER_H