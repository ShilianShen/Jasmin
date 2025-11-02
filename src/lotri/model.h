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
typedef struct LOTRI_MW LOTRI_MW;
typedef struct LOTRI_Model LOTRI_Model;
typedef struct LOTRI_World LOTRI_World;
struct LOTRI_MW {
    LOTRI_Model* model;
    LOTRI_World* world;
};


LOTRI_MW *LOTRI_DeleteMW(LOTRI_MW *mw);
LOTRI_MW* LOTRI_CreateModel(const char* file_obj, const char *file_mtl, ModelSide side);


bool LOTRI_GetModelWorldVertex(const LOTRI_MW* mw, int index, Vec3f* vec);
bool LOTRI_GetModelModelVertex(const LOTRI_MW* mw, int index, Vec3f* vec);
bool LOTRI_GetModelDepth(const LOTRI_MW* mw, float* depth);
bool LOTRI_SetModelScale(LOTRI_MW* mw, Vec3f scale);
bool LOTRI_GetWorldPosition(const LOTRI_World *world, Vec3f* position);
bool LOTRI_SetModelPosition(LOTRI_MW* mw, Vec3f position);
bool LOTRI_SetModelRotation(LOTRI_MW* mw, Vec3f rotation);
bool LOTRI_SetModelNormals(const LOTRI_MW* mw, ModelSide side);
bool LOTRI_SetModelSrc(LOTRI_MW* mw, SDL_FRect* src);


bool LOTRI_RenewModel(LOTRI_MW* mw);


bool LOTRI_DrawModel(const LOTRI_MW* mw);



#endif //MODEL_H