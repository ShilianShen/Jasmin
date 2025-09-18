#ifndef MODEL_H
#define MODEL_H


#include "matrix.h"
#include "camera.h"


typedef struct Model Model;
typedef enum ModelSide {
    MODEL_SIDE_NULL,
    MODEL_SIDE_OUT,
    MODEL_SIDE_IN,
    MODEL_SIDE_CAMERA
} ModelSide;


void LOTRI_DestroyModel(Model* model);
Model* LOTRI_CreateModel(const char* file_obj, const char *file_mtl, ModelSide side);


bool LOTRI_SetModelPosition(Model* model, Vec3f position);
bool LOTRI_SetModelRotation(Model* model, Vec3f rotation);
bool LOTRI_SetModelNormals(const Model* model, ModelSide side);
bool LOTRI_SetModelMat(Model* model, Mat4f mat);


bool LOTRI_GetModelCZ(const Model* model, float* cz);
bool LOTRI_SetModelSrc(Model* model, SDL_FRect* src);


bool LOTRI_RenewModel(Model* model);
bool LOTRI_RenewModelArray(int N, Model* modelArray[N]);

bool LOTRI_DrawModel(const Model* model);
bool LOTRI_DrawModelArray(int N, const Model* modelArray[N]);


#endif //MODEL_H