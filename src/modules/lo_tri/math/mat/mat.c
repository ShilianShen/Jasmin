#include "mat.h"


const Mat4x4f unitMat = {
    {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1},
    }
};


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
void MAT_LoadProductVecMat(const int N, const bool w, Vec3f vecOut[N], const Vec3f vecIn[N], const Mat4x4f mat) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < 3; j++) {
            vecOut[i].v[j] = 0;
            for (int k = 0; k < 3; k++) {
                vecOut[i].v[j] += vecIn[i].v[k] * mat.m[k][j];
            }
            if (w == true) {
                vecOut[i].v[j] += mat.m[3][j];
            }
        }
    }
}


// SRT三剑客
static Mat4x4f MAT_GetMatS(const Vec3f vec) {
    Mat4x4f mat = {0};
    mat.m[0][0] = vec.x;
    mat.m[1][1] = vec.y;
    mat.m[2][2] = vec.z;
    mat.m[3][3] = 1;
    return mat;
}
static Mat4x4f MAT_GetMatRX(const float x) {
    Mat4x4f mat = {0};
    mat.m[0][0] = mat.m[3][3] = 1;
    mat.m[1][1] = mat.m[2][2] = SDL_cosf(x);
    mat.m[2][1] = SDL_sinf(x);
    mat.m[1][2] = -SDL_sinf(x);
    return mat;
}
static Mat4x4f MAT_GetMatRY(const float y) {
    Mat4x4f mat = {0};
    mat.m[1][1] = mat.m[3][3] = 1;
    mat.m[2][2] = mat.m[0][0] = SDL_cosf(y);
    mat.m[0][2] = SDL_sinf(y);
    mat.m[2][0] = -SDL_sinf(y);
    return mat;
}
static Mat4x4f MAT_GetMatRZ(const float z) {
    Mat4x4f mat = {0};
    mat.m[2][2] = mat.m[3][3] = 1;
    mat.m[0][0] = mat.m[1][1] = SDL_cosf(z);
    mat.m[1][0] = SDL_sinf(z);
    mat.m[0][1] = -SDL_sinf(z);
    return mat;
}
static Mat4x4f MAT_GetMatR(const Vec3f vec) {
    // (Z @ Y) @ X = ZY @ X = ZYX
    const Mat4x4f matX = MAT_GetMatRX(vec.x);
    const Mat4x4f matY = MAT_GetMatRY(vec.y);
    const Mat4x4f matZ = MAT_GetMatRZ(vec.z);
    const Mat4x4f matZY = MAT_GetProduct(matZ, matY);
    const Mat4x4f matZYX = MAT_GetProduct(matZY, matX);
    return matZYX;
}
static Mat4x4f MAT_GetMatT(const Vec3f vec) {
    Mat4x4f mat = {0};
    for (int i = 0; i < 4; i++) {
        mat.m[i][i] = 1;
    }
    mat.m[3][0] = vec.x;
    mat.m[3][1] = vec.y;
    mat.m[3][2] = vec.z;
    return mat;
}

Mat4x4f MAT_GetMatSRT(const Vec3f* scale, const Vec3f* rotation, const Vec3f* translation) {
    const Mat4x4f matS = scale != NULL ? MAT_GetMatS(*scale) : unitMat;
    const Mat4x4f matR = rotation != NULL ? MAT_GetMatR(*rotation) : unitMat;
    const Mat4x4f matT = translation != NULL ? MAT_GetMatT(*translation) : unitMat;
    const Mat4x4f matSRT = MAT_GetProduct(MAT_GetProduct(matS, matR), matT);
    return matSRT;
}
Mat4x4f MAT_GetInvSRT(const Vec3f* scale, const Vec3f* rotation, const Vec3f* translation) {
    const Mat4x4f invS = scale != NULL ? MAT_GetMatS(VEC_GetReciprocal(*scale)) : unitMat;
    const Mat4x4f invR = rotation != NULL ? MAT_GetMatR(VEC_GetNegative(*rotation)) : unitMat;
    const Mat4x4f invT = translation != NULL ? MAT_GetMatT(VEC_GetNegative(*translation)) : unitMat;
    const Mat4x4f invSRT = MAT_GetProduct(MAT_GetProduct(invT, invR), invS);
    return invSRT;
}