#include "mat.h"


Mat4x4f MAT_GetProduct(const Mat4x4f matA, const Mat4x4f matB) {
    Mat4x4f matAB = {0};
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            matAB.m[i][j] = 0;
            for (int k = 0; k < 4; k++) {
                matAB.m[i][j] += matA.m[i][k] * matB.m[k][j];
            }
        }
    }
    return matAB;
}


Mat4x4f MAT_GetTranslation(const Vec3f vec) {
    Mat4x4f mat = {0};
    for (int i = 0; i < 4; i++) {
        mat.m[i][i] = 1;
    }
    mat.m[3][0] = vec.x;
    mat.m[3][1] = vec.y;
    mat.m[3][2] = vec.z;
    return mat;
}
static Mat4x4f MAT_GetRotationX(const float x) {
    Mat4x4f mat = {0};
    mat.m[0][0] = mat.m[3][3] = 1;
    mat.m[1][1] = mat.m[2][2] = SDL_cosf(x);
    mat.m[2][1] = SDL_sinf(x);
    mat.m[1][2] = -SDL_sinf(x);
    return mat;
}
static Mat4x4f MAT_GetRotationY(const float y) {
    Mat4x4f mat = {0};
    mat.m[1][1] = mat.m[3][3] = 1;
    mat.m[2][2] = mat.m[0][0] = SDL_cosf(y);
    mat.m[0][2] = SDL_sinf(y);
    mat.m[2][0] = -SDL_sinf(y);
    return mat;
}
static Mat4x4f MAT_GetRotationZ(const float z) {
    Mat4x4f mat = {0};
    mat.m[2][2] = mat.m[3][3] = 1;
    mat.m[0][0] = mat.m[1][1] = SDL_cosf(z);
    mat.m[1][0] = SDL_sinf(z);
    mat.m[0][1] = -SDL_sinf(z);
    return mat;
}
Mat4x4f MAT_GetRotation(const Vec3f vec) {
    // (Z @ Y) @ X = ZY @ X = ZYX
    const Mat4x4f matX = MAT_GetRotationX(vec.x);
    const Mat4x4f matY = MAT_GetRotationY(vec.y);
    const Mat4x4f matZ = MAT_GetRotationZ(vec.z);
    const Mat4x4f matZY = MAT_GetProduct(matZ, matY);
    const Mat4x4f matZYX = MAT_GetProduct(matZY, matX);
    return matZYX;
}
Mat4x4f MAT_GetScale(const Vec3f vec) {
    Mat4x4f mat = {0};
    mat.m[0][0] = vec.x;
    mat.m[1][1] = vec.y;
    mat.m[2][2] = vec.z;
    mat.m[3][3] = 1;
    return mat;
}
Mat4x4f MAT_GetTRS(const Vec3f scale, const Vec3f rotation, const Vec3f translation) {
    const Mat4x4f matS = MAT_GetScale(scale);
    const Mat4x4f matR = MAT_GetRotation(rotation);
    const Mat4x4f matT = MAT_GetTranslation(translation);
    const Mat4x4f matTR = MAT_GetProduct(matT, matR);
    const Mat4x4f matTRS = MAT_GetProduct(matTR, matS);
    return matTRS;
}