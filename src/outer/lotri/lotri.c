#include "lotri.h"

#include "camera.h"
#include "cube.h"



Mat4f matProj;

bool LOTRI_Init() {
    cube.texture = IMG_LoadTexture(renderer, "../res/image/Composition_pour_Jazz.jpg");
    cube.finalVertices[4].tex_coord = (SDL_FPoint){0, 1};
    cube.finalVertices[5].tex_coord = (SDL_FPoint){1, 1};
    cube.finalVertices[6].tex_coord = (SDL_FPoint){1, 0};
    cube.finalVertices[7].tex_coord = (SDL_FPoint){0, 0};
    for (int i = 0; i < cube.numVertices; i++) {
        cube.finalVertices[i].color = (SDL_FColor){1, 1, 1, 1};
    }
    LOTRI_SetModelNormals(&cube, false);
    return true;
}
bool LOTRI_Renew() {
    matProj = (Mat4f){
        .m = {
            {0, 0, -1, -1},
            {1, 0, 0, 0},
            {0, -1, 0, 0},
            {(float)windowWidth / 2, (float)windowHeight / 2, 0, 1},
        }
    };
    LOTRI_RenewCamera();

    const Mat4f matArr[] = {
        LOTRI_GetMatR((Vec3f){0}),
        LOTRI_GetMatS((Vec3f){400, 400, 400}),
        LOTRI_GetInvR(camera.rotation),
        matProj,
    };
    cube.mat = LOTRI_GetProd(sizeof(matArr) / sizeof(Mat4f), matArr);
    LOTRI_RenewModel(&cube);
    return true;
}
bool LOTRI_Draw() {
    LOTRI_DrawModel(&cube);
    return true;
}
void LOTRI_Exit() {}