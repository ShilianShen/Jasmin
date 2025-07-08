#ifndef JASMIN_LOTRI_MODEL_H
#define JASMIN_LOTRI_MODEL_H


#include "mesh/mesh.h"
#include "bone/bone.h"


typedef struct Model {
    int numVertices;
    Vec3f* vertices;
    Vec3f* vertices2;
    int numFaces;
    Vec3i* faces;
    Vec3f translation;
    Vec3f rotation;
} Model;


extern Model testCube;
extern Model testAxisXYZ;


void MODEL_InitTestCube();
void MODEL_DeinitTestCube();


#endif //JASMIN_LOTRI_MODEL_H