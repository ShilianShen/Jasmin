#ifndef MODEL_H
#define MODEL_H


#include "_lotri.h"
#include "camera.h"


typedef struct {Vec3f xyz; Vec4f rgba; Vec2f uv;} LOTRI_Vertex;
typedef struct {Vec3i ijk; Vec3f xyz;} LOTRI_Face;


typedef enum ModelSide {
    MODEL_SIDE_NULL,
    MODEL_SIDE_OUT,
    MODEL_SIDE_IN,
    MODEL_SIDE_CAMERA
} ModelSide;
typedef struct LOTRI_Model LOTRI_Model;
typedef struct LOTRI_World LOTRI_World;





void LOTRI_DestroyModel(LOTRI_Model* model);
LOTRI_Model* LOTRI_CreateModel(const char* file_obj, const char *file_mtl, ModelSide side);


bool LOTRI_GetModelWorldVertex(const LOTRI_Model* model, int index, Vec3f* vec);
bool LOTRI_GetModelModelVertex(const LOTRI_Model* model, int index, Vec3f* vec);
bool LOTRI_GetModelDepth(const LOTRI_Model* model, float* depth);
bool LOTRI_SetModelScale(LOTRI_Model* model, Vec3f scale);
bool LOTRI_GetModelPosition(const LOTRI_Model* model, Vec3f* position);
bool LOTRI_SetModelPosition(LOTRI_Model* model, Vec3f position);
bool LOTRI_SetModelRotation(LOTRI_Model* model, Vec3f rotation);
bool LOTRI_SetModelNormals(const LOTRI_Model* model, ModelSide side);
bool LOTRI_SetModelSrc(LOTRI_Model* model, SDL_FRect* src);


bool LOTRI_RenewModel(LOTRI_Model* model);


bool LOTRI_DrawModel(const LOTRI_Model* model);



#endif //MODEL_H