#include "villa.h"

#include <MacTypes.h>

#include "weather.h"






Model* modelArr[3];

typedef enum VILLA_Direct {
    DIRECT_W,
    DIRECT_A,
    DIRECT_S,
    DIRECT_D,
    NUM_DIRECTS,
} VILLA_Direct;
SDL_FRect direct2rect[NUM_DIRECTS] = {
    [DIRECT_W] = {0, 0, 0.25f, 0.25f},
    [DIRECT_S] = {0, 0.25f, 0.25f, 0.25f},
    [DIRECT_A] = {0, 0.5f, 0.25f, 0.25f},
    [DIRECT_D] = {0, 0.75f, 0.25f, 0.25f},
};


void VILLA_Renew_Camera() {
    static DelayVec3f smallRotate = {0}, bigRotate = {0};
    bigRotate.block = true;
    {
        const float angle = M_PI_4 * 0.5;
        Vec3f v = {0, 0.5f, 0.3f};
        if (DEVICE_GetKeyPressed(SDL_SCANCODE_DOWN )) v.v.y += angle;
        if (DEVICE_GetKeyPressed(SDL_SCANCODE_UP   )) v.v.y -= angle;
        if (DEVICE_GetKeyPressed(SDL_SCANCODE_LEFT )) v.v.z += angle;
        if (DEVICE_GetKeyPressed(SDL_SCANCODE_RIGHT)) v.v.z -= angle;
        LOTRI_SetDelayVec(&smallRotate, v, 1.f);
    }
    {
        static float k = 0;
        Vec3f v = {0, 0, 0};
        if (DEVICE_GetKeyPress(SDL_SCANCODE_Q)) k += 1;
        if (DEVICE_GetKeyPress(SDL_SCANCODE_E)) k -= 1;
        v.v.z += k * (float)M_PI_2;
        LOTRI_SetDelayVec(&bigRotate, v, 0.5f);
    }
    const Vec3f vs = LOTRI_GetDelayVecVec(smallRotate);
    const Vec3f vd = LOTRI_GetDelayVecVec(bigRotate);
    const Vec3f rotate = (Vec3f){
        vs.v.x + vd.v.x,
        vs.v.y + vd.v.y,
        vs.v.z + vd.v.z
    };
    LOTRI_SetCamera(rotate);
}
bool VILLA_Init() {
    modelArr[0] = LOTRI_CreateModel(
        "../res/model/root_room/model.obj",
        "../res/model/root_room/material.mtl",
        MODEL_SIDE_IN
        );
    modelArr[1] = LOTRI_CreateModel(
        "../res/model/character_bw/model.obj",
        "../res/model/character_bw/material.mtl",
        MODEL_SIDE_CAMERA
        );
    modelArr[2] = LOTRI_CreateModel(
        "../res/model/character_ot/model.obj",
        "../res/model/character_ot/material.mtl",
        MODEL_SIDE_CAMERA
        );
    LOTRI_SetModelPosition(modelArr[2], (Vec3f){-1, 1, 0});
    VILLA_InitRain();
    return true;
}
bool VILLA_Renew() {
    VILLA_Renew_Camera();
    return true
    && LOTRI_RenewModelArray(len_of(modelArr), modelArr)
    // && VILLA_RenewRain()
    ;
}
bool VILLA_Draw() {
    float a = 0;
    LOTRI_GetModelCZ(modelArr[1], &a);
    a = loop(0, a, M_PI * 2);
    DEBUG_SendMessageR("%f, %.4f\n", M_PI_4, a);
    if (7 * M_PI_4 < a || a <= 1 * M_PI_4) LOTRI_SetModelSrc(modelArr[1], &direct2rect[DIRECT_W]);
    else if (1 * M_PI_4 < a && a <= 3 * M_PI_4) LOTRI_SetModelSrc(modelArr[1], &direct2rect[DIRECT_A]);
    else if (3 * M_PI_4 < a && a <= 5 * M_PI_4) LOTRI_SetModelSrc(modelArr[1], &direct2rect[DIRECT_S]);
    else if (5 * M_PI_4 < a && a <= 7 * M_PI_4) LOTRI_SetModelSrc(modelArr[1], &direct2rect[DIRECT_D]);
    if (7 * M_PI_4 < a || a <= 1 * M_PI_4) LOTRI_SetModelSrc(modelArr[2], &direct2rect[DIRECT_W]);
    else if (1 * M_PI_4 < a && a <= 3 * M_PI_4) LOTRI_SetModelSrc(modelArr[2], &direct2rect[DIRECT_A]);
    else if (3 * M_PI_4 < a && a <= 5 * M_PI_4) LOTRI_SetModelSrc(modelArr[2], &direct2rect[DIRECT_S]);
    else if (5 * M_PI_4 < a && a <= 7 * M_PI_4) LOTRI_SetModelSrc(modelArr[2], &direct2rect[DIRECT_D]);
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
