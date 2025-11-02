#ifndef LOTRI_BUFFER_H
#define LOTRI_BUFFER_H


#include "model.h"
#include "world.h"
#include "mw.h"


bool LOTRI_ClearBuffer();
bool LOTRI_BufferModel(LOTRI_MW *model);
bool LOTRI_RenewModelBuffer();
bool LOTRI_DrawModelBuffer();


#endif //LOTRI_BUFFER_H