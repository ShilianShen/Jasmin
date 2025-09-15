#include "lotri.h"

#include "camera.h"
#include "cube.h"



Mat4f matProj;

bool LOTRI_Init() {
    cube.texture = IMG_LoadTexture(renderer, "../res/image/Composition_pour_Jazz.jpg");
    cube.finalVertices[1].tex_coord = (SDL_FPoint){0, 1};
    cube.finalVertices[5].tex_coord = (SDL_FPoint){0, 0};
    cube.finalVertices[6].tex_coord = (SDL_FPoint){1, 0};
    cube.finalVertices[2].tex_coord = (SDL_FPoint){1, 1};
    for (int i = 0; i < cube.numVertices; i++) {
        cube.finalVertices[i].color = (SDL_FColor){1, 1, 1, 1};
    }
    LOTRI_SetModelNormals(&cube, false);
    return true;
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
    LOTRI_RenewCamera();

    const Mat4f matArr[] = {
        LOTRI_GetMatS((Vec3f){1, 1, 1}),
        LOTRI_GetMatR((Vec3f){0}),
        LOTRI_GetMatT((Vec3f){0}),
        LOTRI_GetInvT(camera.position),
        LOTRI_GetInvR(camera.rotation),
        matProj,
    };
    cube.mat = LOTRI_GetProd(sizeof(matArr) / sizeof(Mat4f), matArr);
    LOTRI_RenewModel(&cube);
    return true;
}
bool LOTRI_Draw() {
    LOTRI_DrawModel(&cube);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    const SDL_FRect rect = {cube.worldVertices[5].v.x, cube.worldVertices[5].v.y, 10, 10};
    SDL_RenderFillRect(renderer, &rect);
    return true;
}
void LOTRI_Exit() {}