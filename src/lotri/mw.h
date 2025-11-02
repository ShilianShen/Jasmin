#ifndef MW_H
#define MW_H


#include "_lotri.h"
#include "model.h"
#include "world.h"


LOTRI_MW* LOTRI_CreateMW(const char* file_obj, const char *file_mtl, ModelSide side);
LOTRI_MW *LOTRI_DeleteMW(LOTRI_MW *mw);


#endif //MW_H
