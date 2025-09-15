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
            {0, 0, 1, 0},
            {1, 0, 0, 0},
            {0, -1, 0, 0},
            {(float)windowWidth / 2, (float)windowHeight / 2, 0, 1},
        }
    };
    return LOTRI_RenewCamera();
}
bool LOTRI_Draw() {

    const float t = (float)SDL_GetTicks() / 300;
    const Mat4f matArr[] = {
        LOTRI_GetMatR((Vec3f){0}),
        LOTRI_GetMatS(200, 200, 200),
        LOTRI_GetInvR(camera.rotation),
        matProj,
    };
    const Mat4f mat = LOTRI_GetProd(sizeof(matArr) / sizeof(Mat4f), matArr);

    Vec4f vec_after[8];
    LOTRI_LoadVecXMat(8, cube.modelVertices, mat, vec_after);


    for (int i = 0; i < 8; i++) {
        cube.worldVertices[i].position.x = vec_after[i].v.x;
        cube.worldVertices[i].position.y = vec_after[i].v.y;
        cube.worldVertices[i].color = (SDL_FColor){1, 1, 1, 1};
    }

    const int f = 1;

    DEBUG_DrawGeometry(renderer, NULL, cube.worldVertices, cube.numVertex, (int*)cube.indices, cube.numIndices * 3);
    return true;
}
void LOTRI_Exit() {}