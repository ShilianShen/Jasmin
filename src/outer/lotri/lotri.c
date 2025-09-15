#include "lotri.h"

#include "camera.h"
#include "cube.h"



bool LOTRI_Init() {
    return true;
}
bool LOTRI_Renew() {
    return LOTRI_RenewCamera();
}
bool LOTRI_Draw() {

    const float t = (float)SDL_GetTicks() / 300;
    const Mat4f matR = LOTRI_GetMatR(0, 0, 0);
    const Mat4f matS = LOTRI_GetMatS(200, 200, 200);
    const Mat4f invR = LOTRI_GetInvR(camera.rotation.v.x, camera.rotation.v.y, camera.rotation.v.z);
    const Mat4f matSR = LOTRI_GetMatXMat(LOTRI_GetMatXMat(matS, matR), invR);

    Vec4f vec_after[8];
    LOTRI_LoadVecXMat(8, cube.modelVertices, matSR, vec_after);


    for (int i = 0; i < 8; i++) {
        cube.worldVertices[i].position.x = vec_after[i].v.x + (float)windowWidth / 2;
        cube.worldVertices[i].position.y = vec_after[i].v.y + (float)windowHeight / 2;
        cube.worldVertices[i].color = (SDL_FColor){1, 1, 1, 1};
    }

    const int f = 1;

    DEBUG_DrawGeometry(renderer, NULL, cube.worldVertices, cube.numVertex, (int*)cube.indices, cube.numIndices * 3);
    return true;
}
void LOTRI_Exit() {}