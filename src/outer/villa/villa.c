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
    static VILLA_Direct direct = DIRECT_A;
    Vec3f rotation = {0}, position = {0};

    const Vec3f dd[NUM_DIRECTS] = {
        [DIRECT_S] = {0, 0.5f, M_PI_2 * 0},
        [DIRECT_D] = {0, 0.5f, M_PI_2 * 1},
        [DIRECT_W] = {0, 0.5f, M_PI_2 * 2},
        [DIRECT_A] = {0, 0.5f, M_PI_2 * 3},
    };
    if (DEVICE_GetKeyPress(SDL_SCANCODE_Q)) {
        switch (direct) {
            case DIRECT_W: direct = DIRECT_A; break;
            case DIRECT_A: direct = DIRECT_S; break;
            case DIRECT_S: direct = DIRECT_D; break;
            case DIRECT_D: direct = DIRECT_W; break;
            default: break;
        }
    }
    if (DEVICE_GetKeyPress(SDL_SCANCODE_E)) {
        switch (direct) {
            case DIRECT_W: direct = DIRECT_D; break;
            case DIRECT_A: direct = DIRECT_W; break;
            case DIRECT_S: direct = DIRECT_A; break;
            case DIRECT_D: direct = DIRECT_S; break;
            default: break;
        }
    }
    rotation = dd[direct];
    const float angle = M_PI_4 * 0.8;
    if (DEVICE_GetKeyPressed(SDL_SCANCODE_LEFT)) rotation.v.z += angle;
    if (DEVICE_GetKeyPressed(SDL_SCANCODE_RIGHT)) rotation.v.z -= angle;
    if (DEVICE_GetKeyPressed(SDL_SCANCODE_DOWN)) rotation.v.y += angle;
    if (DEVICE_GetKeyPressed(SDL_SCANCODE_UP)) rotation.v.y -= angle;

    LOTRI_SetCamera(rotation, position, 1000);
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
    if (a < 0) a += M_PI * 2;
    if (a > M_PI * 2) a -= M_PI * 2;
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
