#ifndef MODEL_H
#define MODEL_H


#include "matrix.h"


typedef struct Model {
    int numVertices;
    Vec3f* modelVertices; Vec4f* worldVertices;
    SDL_Vertex* finalVertices;

    int numFaces;
    Vec3i* modelFaces;
    Vec3f* modelFaceNormals; Vec4f* worldFaceNormals;

    int* faceIndices;

    float depth;
    SDL_Texture* texture;
    Mat4f mat;
} Model;


bool LOTRI_SetModelNormals(const Model* model, bool in);
bool LOTRI_RenewModel(Model* model);
bool LOTRI_DrawModel(const Model* model);


#endif //MODEL_H