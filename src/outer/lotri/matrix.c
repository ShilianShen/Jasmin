#include "matrix.h"



Mat4f LOTRI_GetMatXMat(const Mat4f A, const Mat4f B) {
    Mat4f AB = {0};
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                AB.m[i][j] += A.m[i][k] * B.m[k][j];
            }
        }
    }
    return AB;
}
Mat4f LOTRI_GetProd(const int N, Mat4f matArray[N]) {
    Mat4f result = matArray[0];
    for (int i = 1; i < N; i++) {
        result = LOTRI_GetMatXMat(result, matArray[i]);
    }
    return result;
}


static Mat4f LOTRI_GetMatRX(const float x) {
    const Mat4f result = {
        {
            {1, 0, 0, 0},
            {0, SDL_cosf(x), -SDL_sinf(x), 0},
            {0, SDL_sinf(x), SDL_cosf(x), 0},
            {0, 0, 0, 1}
        }
    };
    return result;
}
static Mat4f LOTRI_GetMatRY(const float y) {
    const Mat4f result = {
        {
            {SDL_cosf(y), 0, SDL_sinf(y), 0},
            {0, 1, 0, 0},
            {-SDL_sinf(y), 0, SDL_cosf(y), 0},
            {0, 0, 0, 1}
        }
    };
    return result;
}
static Mat4f LOTRI_GetMatRZ(const float z) {
    const Mat4f result = {
        {
            {SDL_cosf(z), -SDL_sinf(z), 0, 0},
            {SDL_sinf(z), SDL_cosf(z), 0, 0},
            {0, 0, 1, 0},
            {0, 0, 0, 1}
        }
    };
    return result;
}
Mat4f LOTRI_GetMatR(const Vec3f vec) {
    const Mat4f M[] = {
        LOTRI_GetMatRZ(vec.v.z),
        LOTRI_GetMatRY(vec.v.y),
        LOTRI_GetMatRX(vec.v.x),
    };
    return LOTRI_GetProd(sizeof(M) / sizeof(Mat4f), M);
}
Mat4f LOTRI_GetInvR(const Vec3f vec) {
    const Mat4f M[] = {
        LOTRI_GetMatRX(-vec.v.x),
        LOTRI_GetMatRY(-vec.v.y),
        LOTRI_GetMatRZ(-vec.v.z),
    };
    return LOTRI_GetProd(sizeof(M) / sizeof(Mat4f), M);
}
Mat4f LOTRI_GetMatS(const float x, const float y, const float z) {
    const Mat4f result = {
        {
            {x, 0, 0, 0},
            {0, y, 0, 0},
            {0, 0, z, 0},
            {0, 0, 0, 1}
        }
    };
    return result;

}


bool LOTRI_LoadVecXMat(const int N, Vec4f vec_in[N], const Mat4f mat, Vec4f vec_out[N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < 4; j++) {
            vec_out[i].arr[j] = 0;
            for (int k = 0; k < 4; k++) {
                vec_out[i].arr[j] += vec_in[i].arr[k] * mat.m[k][j];
            }
        }
    }
    return true;
}