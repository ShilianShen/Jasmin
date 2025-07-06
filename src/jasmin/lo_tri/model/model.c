#include "model.h"


Model testCube;



void MODEL_InitTestCube() {
    // init vertices
    testCube.numVertices = 36;
    testCube.vertices = malloc(testCube.numVertices * sizeof(Vec3f));
    testCube.vertices2 = malloc(testCube.numVertices * sizeof(Vec3f));
    {
        float a = 800;
        const float vertices[36][3] = {
            {-0.5f, -0.5f, -0.5f},
            {0.5f, -0.5f, -0.5f},
            {0.5f,  0.5f, -0.5f},
            {0.5f,  0.5f, -0.5f},
            {-0.5f,  0.5f, -0.5f},
            {-0.5f, -0.5f, -0.5f},

            {-0.5f, -0.5f,  0.5f},
            { 0.5f, -0.5f,  0.5f},
            {0.5f,  0.5f,  0.5f},
            {0.5f,  0.5f,  0.5f},
            {-0.5f,  0.5f,  0.5f},

            {-0.5f,  0.5f,  0.5f},
            {-0.5f,  0.5f, -0.5f},
            { -0.5f, -0.5f, -0.5f},
            {-0.5f, -0.5f, -0.5f},
            {-0.5f, -0.5f,  0.5f},
            {-0.5f,  0.5f,  0.5f},

            {0.5f,  0.5f,  0.5f},
            {0.5f,  0.5f, -0.5f},
            {0.5f, -0.5f, -0.5f},
            {0.5f, -0.5f, -0.5f},
            {0.5f, -0.5f,  0.5f},
            { 0.5f,  0.5f,  0.5f},

            {-0.5f, -0.5f, -0.5f},
            {0.5f, -0.5f, -0.5f},
            {0.5f, -0.5f,  0.5f},
            {0.5f, -0.5f,  0.5f},
            {-0.5f, -0.5f,  0.5f},
            {-0.5f, -0.5f, -0.5f},

            {-0.5f,  0.5f, -0.5f},
            {0.5f,  0.5f, -0.5f},
            {0.5f,  0.5f,  0.5f},
            {0.5f,  0.5f,  0.5f},
            {-0.5f,  0.5f,  0.5f},
            {-0.5f,  0.5f, -0.5f},
        };
        for (int i = 0; i < testCube.numVertices; i++) {
            testCube.vertices[i].x = vertices[i][0] * a;
            testCube.vertices[i].y = vertices[i][1] * a;
            testCube.vertices[i].z = vertices[i][2] * a;
        }
    }
    // init faces
    testCube.numFaces = 12;
    testCube.faces = malloc(testCube.numFaces * sizeof(Vec3i));
    for (int i = 0; i < testCube.numFaces; i++) {
        testCube.faces[i].a = 3 * i + 0;
        testCube.faces[i].b = 3 * i + 1;
        testCube.faces[i].c = 3 * i + 2;
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