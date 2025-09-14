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
    const Matrix mat = LOTRI_LoadR(t, 0, 0);

    vec4f vec_after[8];
    LOTRI_Trans(8, cube.modelVertices, mat, vec_after);

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