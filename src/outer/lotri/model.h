#ifndef MODEL_H
#define MODEL_H


#include "matrix.h"
#include "camera.h"


extern Mat4f matProj;


typedef struct Model Model;


void LOTRI_DestroyModel(Model* model);
Model* LOTRI_CreateModel(const char* file_obj, const char *file_mtl);


bool LOTRI_SetModelPosition(Model* model, Vec3f position);
bool LOTRI_SetModelRotation(Model* model, Vec3f rotation);
bool LOTRI_SetModelNormals(const Model* model, bool in);
bool LOTRI_SetModelMat(Model* model, Mat4f mat);


bool LOTRI_RenewModel(Model* model);
bool LOTRI_DrawModel(const Model* model);


#endif //MODEL_H