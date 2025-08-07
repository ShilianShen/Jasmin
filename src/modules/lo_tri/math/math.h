#ifndef JASMIN_LOTRI_MATH_H
#define JASMIN_LOTRI_MATH_H


#include "mat/mat.h"


typedef struct Face {
    bool on;
    int indices[3];
    Vec3f z;
    Vec3f normal;
    SDL_Vertex vertices[3];
} Face;


#endif //JASMIN_LOTRI_MATH_H