#ifndef MESH_H
#define MESH_H


#include "../interface.h"


typedef struct Mesh {

    struct Vertices {
        const int num;  // file
        Vec3f* points;  // file, N*4 * float
        float* weights; // file
    } vertices;

    struct Indices {
        const int num; // file
        Vec3i* faces;  // file, N*3 * int
        Vec3f* norms;  // init, N*4 * float
    } indices;

    Mat4x4f tran; // renew

} Mesh;


void MESH_InitNorms(const Mesh* mesh);


#endif //MESH_H
