#include "villa.h"
#include "weather.h"



Model* modelArr[3];


bool VILLA_Init() {
    modelArr[0] = LOTRI_CreateModel("../res/model/root_room/model.obj", "../res/model/root_room/material.mtl", true);
    modelArr[1] = LOTRI_CreateModel("../res/model/cube/model.obj", "../res/model/cube/material.mtl", false);
    modelArr[2] = LOTRI_CreateModel("../res/model/cube/model.obj", "../res/model/cube/material.mtl", false);

    LOTRI_SetModelPosition(modelArr[1], (Vec3f){0, 0, 1});

    VILLA_InitRain();
    return true;
}
bool VILLA_Renew() {
    const float t = (float)SDL_GetTicks() / 500;
    return true
    && LOTRI_SetModelPosition(modelArr[2], (Vec3f){1.5f * SDL_cosf(t), 1.5f * SDL_sinf(t), 1.5f})
    && LOTRI_RenewModelArray(len_of(modelArr), modelArr)
    && VILLA_RenewRain()
    ;
}
bool VILLA_Draw() {
    return true
    && LOTRI_DrawModelArray(len_of(modelArr), modelArr)
    // && VILLA_DrawRain()
    ;
}
void VILLA_Exit() {
    for (int i = 0; i < len_of(modelArr); i++) {
        LOTRI_DestroyModel(modelArr[i]);
    }
}
