#ifndef MODEL_H
#define MODEL_H


#include "matrix.h"
#include "camera.h"


extern Mat4f matProj;


typedef struct Model Model;


void LOTRI_DestroyModel(Model* model);
Model* LOTRI_CreateModel(const char* filename);


bool LOTRI_SetModelNormals(const Model* model, bool in);
bool LOTRI_SetModelVertices(const Model* model, int N, const Vec3f xyz[N]);
bool LOTRI_SetModelFaces(const Model* model, int N, const Vec3i abc[N]);
bool LOTRI_SetModelTexture(Model* model, const char* filename);
bool LOTRI_SetModelUV(const Model* model, int N, const Vec2f uv[N]);
bool LOTRI_SetModelMat(Model* model, Mat4f mat);


bool LOTRI_RenewModel(Model* model);
bool LOTRI_DrawModel(const Model* model);


#endif //MODEL_H