#ifndef JASMIN_LOTRI_MODEL_H
#define JASMIN_LOTRI_MODEL_H


#include "mesh/mesh.h"
#include "bone/bone.h"


typedef struct Model {
    // vertices
    int numVertices;
    Vec3f* vertices; // malloc
    // faces
    int numFaces;
    Face* faces;
    // tran
    Vec3f translation;
    Vec3f rotation;
} Model;


extern Model testCube;
extern Model testAxisXYZ;


void MODEL_InitTestCube();
void MODEL_DeinitTestCube();


#endif //JASMIN_LOTRI_MODEL_H