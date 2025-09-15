#ifndef MODEL_H
#define MODEL_H


#include "matrix.h"


typedef struct Model {
    int numVertex;
    Vec4f* modelVertices;
    Vec4f* worldVertices;
    SDL_Vertex* finalVertices;

    int numIndices;
    int shape;
    Vec3i* indices;
} Model;


#endif //MODEL_H