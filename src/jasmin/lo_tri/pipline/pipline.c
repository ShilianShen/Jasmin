#include "pipline.h"


Mat4x4f view, proj;


static Mat4x4f PIPLINE_GetWorld(const Vec3f translation, const Vec3f rotation) {
    const Mat4x4f world = MAT_GetMatSRT(NULL, &rotation, &translation);
    return world;
}
static Mat4x4f PIPLINE_GetTran(const Vec3f translation, const Vec3f rotation) {
    const Mat4x4f tran = MAT_GetProduct(
        PIPLINE_GetWorld(translation, rotation),
        MAT_GetProduct(view, proj)
    );
    return tran;
}


static void PIPLINE_RenewView() {
    view = MAT_GetInvSRT(
        NULL,
        &camera.rotation,
        &camera.translation
    );
}
static void PIPLINE_RenewProj() {
    proj = (Mat4x4f){
        {
            {0, 0, -1, 0},
            {1, 0, 0, 0},
            {0, -1, 0, 0},
            {(float)windowWidth / 2, (float)windowHeight / 2, 0, 1},
        }
    };
}
static void PIPLINE_RenewModel(const Model model) {
    // get tran
    const Mat4x4f tran = PIPLINE_GetTran(model.translation, model.rotation);

    // get vertices
    Vec3f vertices[model.numVertices];
    MAT_LoadProductVecMat(model.numVertices, 1, vertices, model.vertices, tran);

    // get normals
    Vec3f model_normals[model.numFaces], normals[model.numFaces];
    for (int i = 0; i < model.numFaces; i++) {model_normals[i] = model.faces[i].normal;}
    MAT_LoadProductVecMat(model.numFaces, 1, normals, model_normals, tran);

    // load
    for (int i = 0; i < model.numFaces; i++) {
        model.faces[i].on = normals[i].z < 0;
        for (int j = 0; j < 3; j++) {
            model.faces[i].vertices[j].position.x = vertices[model.faces[i].indices[j]].x;
            model.faces[i].vertices[j].position.y = vertices[model.faces[i].indices[j]].y;
            model.faces[i].z.v[j] = vertices[model.faces[i].indices[j]].z;
        }
    }
}
void PIPLINE_Renew() {
    PIPLINE_RenewView();
    PIPLINE_RenewProj();
    PIPLINE_RenewModel(testCube);
    printf("%d\n", SDL_GetVersion());
}