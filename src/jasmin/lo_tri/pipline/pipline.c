#include "pipline.h"


Mat4x4f view;
Mat4x4f proj;


void PIPLINE_RenewView() {
    view = MAT_GetInvSRT(
        NULL,
        &camera.rotation,
        &camera.translation
    );
}
void PIPLINE_RenewProj() {
    proj = (Mat4x4f){
        {
            {0, 0, -1, 0},
            {1, 0, 0, 0},
            {0, -1, 0, 0},
            {(float)windowWidth / 2, (float)windowHeight / 2, 0, 1},
        }
    };
}
Mat4x4f PIPLINE_GetWorld(const Vec3f translation, const Vec3f rotation) {
    const Mat4x4f world = MAT_GetMatSRT(NULL, &rotation, &translation);
    return world;
}
Mat4x4f PIPLINE_GetTran(const Vec3f translation, const Vec3f rotation) {
    const Mat4x4f tran = MAT_GetProduct(
        PIPLINE_GetWorld(translation, rotation),
        MAT_GetProduct(view, proj)
    );
    return tran;
}


void PIPLINE_RenewModelVertices(const Model model) {
    const Mat4x4f tran = PIPLINE_GetTran(model.translation, model.rotation);
    MAT_LoadProductVecMat(model.numVertices, true, model.vertices2, model.vertices, tran);
}
void PIPLINE_Draw(const Model model) {
    float v[3][2];
    for (int i = 0; i < model.numFaces; i++) {
        Vec3i f = model.faces[i];
        for (int j = 0; j < 3; j++) {
            v[j][0] = model.vertices2[f.v[j]].x;
            v[j][1] = model.vertices2[f.v[j]].y;
            // DEBUG_SendMessageR("Vec3f: %.2f, %.2f\n", v[j][0], v[j][1]);
        }
        DEBUG_DrawFace(v);
    }
}