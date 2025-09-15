#include "lotri.h"

#include "camera.h"
#include "cube.h"


Mat4f matProj;

bool LOTRI_Init() {
    return true;
}
bool LOTRI_Renew() {
    matProj = (Mat4f){
        .m = {
            {0, 0, -1, -1},
            {1, 0, 0, 0},
            {0, -1, 0, 0},
            {(float)windowWidth / 2, (float)windowHeight / 2, 0, 1},
        }
    };
    return LOTRI_RenewCamera();
}
bool LOTRI_Draw() {

    const Mat4f matArr[] = {
        LOTRI_GetMatR((Vec3f){0}),
        LOTRI_GetMatS((Vec3f){200, 200, 200}),
        LOTRI_GetInvR(camera.rotation),
        matProj,
    };
    const Mat4f mat = LOTRI_GetProd(sizeof(matArr) / sizeof(Mat4f), matArr);

    LOTRI_LoadVecXMat(cube.numVertex, cube.modelVertices, mat, cube.worldVertices);

    for (int i = 0; i < cube.numVertex; i++) {
        cube.finalVertices[i].position.x = cube.worldVertices[i].v.x;
        cube.finalVertices[i].position.y = cube.worldVertices[i].v.y;
        // cube.finalVertices[i].color = (SDL_FColor){1, 1, 1, 1};
    }

    DEBUG_DrawGeometry(renderer, NULL, cube.finalVertices, cube.numVertex, (int*)cube.indices, cube.numIndices * 3);
    return true;
}
void LOTRI_Exit() {}