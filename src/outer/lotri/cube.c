#include "cube.h"


Model* cube = NULL;
Vec3f cubeModelVertices[8] = {
    {-0.5f, -0.5f, -0.5f},  // 0 - 左下后
    { 0.5f, -0.5f, -0.5f},  // 1 - 右下后
    { 0.5f,  0.5f, -0.5f},  // 2 - 右上后
    {-0.5f,  0.5f, -0.5f},  // 3 - 左上后
    {-0.5f, -0.5f,  0.5f},  // 4 - 左下前
    { 0.5f, -0.5f,  0.5f},  // 5 - 右下前
    { 0.5f,  0.5f,  0.5f},  // 6 - 右上前
    {-0.5f,  0.5f,  0.5f}   // 7 - 左上前
};
Vec3i cubeModelFaces[12] = {
    // 后面（z = -0.5）
    {0, 1, 2},
    {2, 3, 0},
    // 前面（z = +0.5）
    {4, 5, 6},
    {6, 7, 4},
    // 左面（x = -0.5）
    {0, 3, 7},
    {7, 4, 0},
    // 右面（x = +0.5）
    {1, 5, 6},
    {6, 2, 1},
    // 底面（y = -0.5）
    {0, 4, 5},
    {5, 1, 0},
    // 顶面（y = +0.5）
    {3, 2, 6},
    {6, 7, 3}
};
Vec2f cubeUV[8] = {
    [1] = {0, 1},
    [5] = {0, 0},
    [6] = {1, 0},
    [2] = {1, 1},
};

bool LOTRI_InitCube() {
    cube = LOTRI_CreateModel(8, 12);
    if (cube == NULL) return false;

    if (
        LOTRI_SetModelTexture(cube, "../res/image/Composition_pour_Jazz.jpg") &&
        LOTRI_SetModelVertices(cube, 8, cubeModelVertices) &&
        LOTRI_SetModelUV(cube, 8, cubeUV) &&
        LOTRI_SetModelFaces(cube, 12, cubeModelFaces) &&
        LOTRI_SetModelNormals(cube, false)) {
    }
    else {
        return false;
    }
    for (int i = 0; i < cube->numVertices; i++) {
        cube->finalVertices[i].color = (SDL_FColor){1, 1, 1, 1};
    }
    return true;
}