#ifndef MODEL_H
#define MODEL_H


#include "matrix.h"
#include "camera.h"


extern Mat4f matProj;


typedef struct Model Model;


void LOTRI_DestroyModel(Model* model);
Model* LOTRI_CreateModel(const char* filename);


bool LOTRI_SetModelNormals(const Model* model, bool in);
bool LOTRI_SetModelTexture(Model* model, const char* filename);
bool LOTRI_SetModelMat(Model* model, Mat4f mat);


bool LOTRI_RenewModel(Model* model);
bool LOTRI_DrawModel(const Model* model);


#endif //MODEL_H