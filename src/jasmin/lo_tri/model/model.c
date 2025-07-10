#include "model.h"


Model testCube;
Model testAxisXYZ;


void MODEL_InitTestCube() {
    // init vertices
    testCube.numVertices = 8;
    testCube.vertices = malloc(testCube.numVertices * sizeof(Vec3f)); // malloc
    {
        const float vertices[8][3] = {
            {+1.f, +1.f, +1.f},
            {+1.f, +1.f, -1.f},
            {+1.f, -1.f, +1.f},
            {+1.f, -1.f, -1.f},

            {-1.f, +1.f, +1.f},
            {-1.f, +1.f, -1.f},
            {-1.f, -1.f, +1.f},
            {-1.f, -1.f, -1.f},
        };
        for (int i = 0; i < testCube.numVertices; i++) {
            const float a = 400;
            testCube.vertices[i].x = vertices[i][0] * a;
            testCube.vertices[i].y = vertices[i][1] * a;
            testCube.vertices[i].z = vertices[i][2] * a;
        }
    }
    // init faces
    testCube.numFaces = 12;
    testCube.faces = malloc(testCube.numFaces * sizeof(Face)); // malloc
    {
        const int indices[12][3] = {
            {0, 1, 2},
            {1, 2, 3},
            {4, 5, 6},
            {5, 6, 7},

            {0, 1, 4},
            {1, 4, 5},
            {2, 3, 6},
            {3, 6, 7},

            {0, 2, 4},
            {2, 4, 6},
            {1, 3, 5},
            {3, 5, 7},
        };
        for (int i = 0; i < testCube.numFaces; i++) {
            for (int j = 0; j < 3; j++) {
                testCube.faces[i].indices[j] = indices[i][j];
            }
        }
    }
    for (int i = 0; i < testCube.numFaces; i++) {
        const Face face = testCube.faces[i];
        const Vec3f vecA = testCube.vertices[face.indices[0]];
        const Vec3f vecB = testCube.vertices[face.indices[1]];
        const Vec3f vecC = testCube.vertices[face.indices[2]];
        testCube.faces[i].normal = VEC_GetNormal(vecA, vecB, vecC);
    }
    // init translation & rotation
    testCube.translation = (Vec3f){0};
    testCube.rotation = (Vec3f){0};
}
void MODEL_DeinitTestCube() {
    // deinit vertices
    free(testCube.vertices);
    testCube.vertices = NULL;
    testCube.numVertices = 0;
    // deinit faces
    free(testCube.faces);
    testCube.faces = NULL;
    testCube.numFaces = 0;
}