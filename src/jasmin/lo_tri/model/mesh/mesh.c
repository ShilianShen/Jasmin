#include "mesh.h"


void MESH_InitNorms(const Mesh* mesh) {
    for (int i = 0; i < mesh->indices.num; i++) {
        const Vec3f vecA = mesh->vertices.points[mesh->indices.faces[i].a];
        const Vec3f vecB = mesh->vertices.points[mesh->indices.faces[i].b];
        const Vec3f vecC = mesh->vertices.points[mesh->indices.faces[i].c];
        mesh->indices.norms[i] = VEC_GetNormal(vecA, vecB, vecC);
    }
}