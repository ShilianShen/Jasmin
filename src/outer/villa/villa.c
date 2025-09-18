#include "villa.h"
#include "weather.h"



Model* modelArr[2];


bool VILLA_Init() {
    modelArr[0] = LOTRI_CreateModel(
        "../res/model/root_room/model.obj",
        "../res/model/root_room/material.mtl",
        MODEL_SIDE_IN
        );
    modelArr[1] = LOTRI_CreateModel(
        "../res/model/character_bw/model.obj",
        "../res/model/character_bw/material.mtl",
        MODEL_SIDE_NULL
        );

    VILLA_InitRain();
    return true;
}
bool VILLA_Renew() {
    return true
    && LOTRI_RenewModelArray(len_of(modelArr), modelArr)
    // && LOTRI_SS(modelArr[1]);
    // && VILLA_RenewRain()
    ;
}
bool VILLA_Draw() {
    return true
    && LOTRI_DrawModelArray(len_of(modelArr), modelArr)
    && VILLA_DrawRain()
    ;
}
void VILLA_Exit() {
    for (int i = 0; i < len_of(modelArr); i++) {
        LOTRI_DestroyModel(modelArr[i]);
    }
}
