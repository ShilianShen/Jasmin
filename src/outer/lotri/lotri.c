#include "lotri.h"

#include "cube.h"


Mat4f matProj;


bool LOTRI_Init() {
    LOTRI_InitCube();
    return true;
}

bool LOTRI_Renew() {
    matProj = (Mat4f){
        .m = {
            {0, 0, 1, 1},
            {-500, 0, 0, 0},
            {0, -500, 0, 0},
            {0, 0, 0, 0},
        }
    };
    LOTRI_RenewCamera();
    LOTRI_RenewModel(cube);
    return true;
}
bool LOTRI_Draw() {
    LOTRI_DrawModel(cube);
    return true;
}
void LOTRI_Exit() {
    LOTRI_DestroyModel(cube);
}