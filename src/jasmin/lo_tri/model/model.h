#ifndef JASMIN_LOTRI_MODEL_H
#define JASMIN_LOTRI_MODEL_H


#include "mesh/mesh.h"
#include "bone/bone.h"


typedef struct Model {
    int num; // file
    int num_bones;  // file
    Mesh* meshes;   // file
    Bone* bones;    // file
} Model;


#endif //JASMIN_LOTRI_MODEL_H