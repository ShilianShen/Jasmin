#ifndef MODEL_H
#define MODEL_H


#include "matrix.h"


typedef struct Model {
    int numVertex;
    Vec4f* modelVertices;
    SDL_Vertex* worldVertices;

    int numIndices;
    int shape;
    Vec3i* indices;
} Model;


#endif //MODEL_H