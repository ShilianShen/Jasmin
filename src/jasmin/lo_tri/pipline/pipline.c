#include "pipline.h"


Mat4x4f view;
Mat4x4f proj;


void PIPLINE_RenewView() {
    view = MAT_GetProduct(
        MAT_GetRotation(camera.rotation),
        MAT_GetTranslation(camera.translation)
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
    const Mat4x4f world = MAT_GetProduct(
        MAT_GetRotation(rotation),
        MAT_GetTranslation(translation)
        );
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
    // (N * 3) * (3 * 3)
    for (int i = 0; i < model.numVertices; i++) {
        for (int j = 0; j < 3; j++) {
            model.vertices2[i].v[j] = 0;
            for (int k = 0; k < 3; k++) {
                model.vertices2[i].v[j] += model.vertices[i].v[k] * tran.m[k][j];
            }
            model.vertices2[i].v[j] += tran.m[3][j];
        }
    }
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