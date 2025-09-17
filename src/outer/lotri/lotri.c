#include "lotri.h"


Mat4f matProj;


bool LOTRI_Init() {
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
    return true;
}
bool LOTRI_Draw() {
    return true;
}
void LOTRI_Exit() {
}