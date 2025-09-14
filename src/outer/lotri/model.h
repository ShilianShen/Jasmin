#ifndef MODEL_H
#define MODEL_H


#include "matrix.h"


typedef struct Model {
    int numVertex;
    vec4f* modelVertices;
    SDL_Vertex* worldVertices;

    int numIndices;
    int shape;
    vec4i* indices;
} Model;


#endif //MODEL_H