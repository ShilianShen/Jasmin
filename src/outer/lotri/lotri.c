#include "lotri.h"

#include "camera.h"
#include "cube.h"



Mat4f matProj;

bool LOTRI_Init() {
    return LOTRI_InitCube();;
}
bool LOTRI_Renew() {
    matProj = (Mat4f){
        .m = {
            {0, 0, 1, 1},
            {-500, 0, 0, 0},
            {0, -500, 0, 0},
            {(float)windowWidth / 2, (float)windowHeight / 2, 0, 0},
        }
    };
    LOTRI_RenewCamera();

    const Mat4f matArr[] = {
        LOTRI_GetMatS((Vec3f){1, 1, 1}),
        LOTRI_GetMatR((Vec3f){0}),
        LOTRI_GetMatT((Vec3f){0}),
        LOTRI_GetInvT(camera.position),
        LOTRI_GetInvR(camera.rotation),
        matProj,
    };

    cube->mat = LOTRI_GetProd(sizeof(matArr) / sizeof(Mat4f), matArr);
    LOTRI_RenewModel(cube);
    return true;
}
bool LOTRI_Draw() {
    LOTRI_DrawModel(cube);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    return true;
}
void LOTRI_Exit() {}