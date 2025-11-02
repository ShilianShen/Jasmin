#ifndef MW_H
#define MW_H


#include "_lotri.h"
#include "model.h"
#include "world.h"


typedef struct LOTRI_MW LOTRI_MW;
struct LOTRI_MW {
    LOTRI_Model* model;
    LOTRI_World* world;
};


LOTRI_MW* LOTRI_CreateMW(const char* file_obj, const char *file_mtl, ModelSide side);
LOTRI_MW *LOTRI_DeleteMW(LOTRI_MW *mw);


#endif //MW_H
