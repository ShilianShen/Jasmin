#include "lotri.h"
#include "cube.h"


bool LOTRI_Init() {

    return true;
}
bool LOTRI_Renew() {


    return true;
}
bool LOTRI_Draw() {

    const float t = (float)SDL_GetTicks() / 100;
    const Matrix matR = LOTRI_LoadR(0, 0, t);
    const Matrix matS = LOTRI_LoadS(100, 100, 100);
    const Matrix mat = LOTRI_MultipleMatrix(matS, matR);

    vec4f vec_after[8];
    LOTRI_Trans(8, cube.modelVertices, mat, vec_after);

    DEBUG_SendMessageR("%f, %f, %f\n", vec_after[0].v.x, vec_after[0].v.y, vec_after[0].v.z);

    for (int i = 0; i < 8; i++) {
        cube.worldVertices[i].position.x = vec_after[i].v.x + (float)windowWidth / 2;
        cube.worldVertices[i].position.y = vec_after[i].v.y + (float)windowHeight / 2;
        cube.worldVertices[i].color = (SDL_FColor){1, 1, 1, 1};
    }

    const int f = 1;


    SDL_RenderGeometry(renderer, NULL, cube.worldVertices, cube.numVertex, cube.indices[f].arr, cube.shape);
    return true;
}
void LOTRI_Exit() {}