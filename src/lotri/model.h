#ifndef MODEL_H
#define MODEL_H


#include "_lotri.h"
#include "camera.h"


typedef enum ModelSide {
    MODEL_SIDE_NULL,
    MODEL_SIDE_OUT,
    MODEL_SIDE_IN,
    MODEL_SIDE_CAMERA,
    NUM_MODEL_SIDES
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
LOTRI_Model* LOTRI_CreateModel(const char* file_obj, const char* file_mtl, ModelSide side);
LOTRI_Model* LOTRI_DeleteModel(LOTRI_Model* model);


#endif //MODEL_H