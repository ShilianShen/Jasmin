#include "lo_tri.h"


#include "pipline/pipline.h"


void LOTRI_Init(SDL_Renderer* renderer) {
    ZBUFFER_Init(renderer);
    MODEL_InitTestCube();
}
void LOTRI_Deinit() {
    MODEL_DeinitTestCube();
    ZBUFFER_Deinit();
}

void LOTRI_Load() {}
void LOTRI_Unload() {}

void LOTRI_Renew() {
    PIPLINE_Renew();
    ZBUFFER_Renew();
    LOTRI_RenewCamera();
}
void LOTRI_Draw() {
    ZBUFFER_SendFaces(testCube.numFaces, testCube.faces);
    ZBUFFER_Draw();
}