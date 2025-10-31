#ifndef MODEL_H
#define MODEL_H


#include "_lotri.h"
#include "camera.h"


typedef struct {
    float t1, t2;
    Vec3f v1, v2;
    AtvFunc func;
    bool block;
} DelayVec3f;


typedef struct {Vec3f xyz; Vec4f rgba; Vec2f uv;} LOTRI_Vertex;
typedef struct {Vec3i ijk; Vec3f xyz;} LOTRI_Face;


typedef struct LOTRI_Model LOTRI_Model;
typedef struct LOTRI_World LOTRI_World;
typedef enum ModelSide {
    MODEL_SIDE_NULL,
    MODEL_SIDE_OUT,
    MODEL_SIDE_IN,
    MODEL_SIDE_CAMERA
} ModelSide;


#define MAX_MODEL_BUFFER 64
extern int modelBufferHead;
extern const LOTRI_Model* modelBuffer[];


void LOTRI_DestroyModel(LOTRI_Model* model);
LOTRI_Model* LOTRI_CreateModel(const char* file_obj, const char *file_mtl, ModelSide side);


bool LOTRI_SetModelScale(LOTRI_Model* model, Vec3f scale);
bool LOTRI_GetModelPosition(const LOTRI_Model* model, Vec3f* position);
bool LOTRI_SetModelPosition(LOTRI_Model* model, Vec3f position);
bool LOTRI_SetModelRotation(LOTRI_Model* model, Vec3f rotation);
bool LOTRI_SetModelNormals(const LOTRI_Model* model, ModelSide side);


bool LOTRI_GetModelWorldVertex(const LOTRI_Model* model, int index, Vec3f* vec);
bool LOTRI_GetModelModelVertex(const LOTRI_Model* model, int index, Vec3f* vec);

bool LOTRI_SetModelSrc(LOTRI_Model* model, SDL_FRect* src);


// bool LOTRI_RenewModel(LOTRI_Model* model);


bool LOTRI_DrawModel(const LOTRI_Model* model);
bool LOTRI_DrawModelBuffer(int N, const LOTRI_Model* modelArray[N]);


#endif //MODEL_H