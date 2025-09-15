#include "lotri.h"

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
    return LOTRI_RenewCamera() && LOTRI_RenewModel(cube);
    return true;
}
bool LOTRI_Draw() {
    LOTRI_DrawModel(cube);
    return true;
}
void LOTRI_Exit() {}