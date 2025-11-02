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
struct LOTRI_Model {
    int numVertices;
    LOTRI_Vertex *vertices;
    int numFaces;
    LOTRI_Face *faces;
    ModelSide side;
    SDL_Texture* texture;
};


bool LOTRI_GetModelVertex(const LOTRI_Model *model, int index, Vec3f* vec);
LOTRI_Model* LOTRI_CreateModel(const fastObjMesh* mesh, const char* file_mtl, ModelSide side);
LOTRI_Model* LOTRI_DeleteModel(LOTRI_Model* model);


#endif //MODEL_H