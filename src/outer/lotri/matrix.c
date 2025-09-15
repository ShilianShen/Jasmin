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


Mat4f LOTRI_GetMatS(const Vec3f vec) {
    const Mat4f result = {
        {
            {vec.v.x, 0, 0, 0},
            {0, vec.v.y, 0, 0},
            {0, 0, vec.v.z, 0},
            {0, 0, 0, 1}
        }
    };
    return result;

}


Vec3f LOTRI_GetNormal(const Vec3f A, const Vec3f B, const Vec3f C) {
    const Vec3f AB = {B.v.x - A.v.x, B.v.y - A.v.y, B.v.z - A.v.z};  // 计算 AB 向量
    const Vec3f AC = {C.v.x - A.v.x, C.v.y - A.v.y, C.v.z - A.v.z};  // 计算 AC 向量

    // 计算叉积
    Vec3f normal;
    normal.v.x = AB.v.y * AC.v.z - AB.v.z * AC.v.y;
    normal.v.y = AB.v.z * AC.v.x - AB.v.x * AC.v.z;
    normal.v.z = AB.v.x * AC.v.y - AB.v.y * AC.v.x;

    return normal;
}
Vec3f LOTRI_GetSum(const Vec3f A, const Vec3f B, const Vec3f C) {
    const Vec3f result = {
        A.v.x + B.v.x + C.v.x,
        A.v.y + B.v.y + C.v.y,
        A.v.z + B.v.z + C.v.z,
    };
    return result;
}
float LOTRI_GetDot(const Vec3f A, const Vec3f B) {
    return A.v.x * B.v.x + A.v.y * B.v.y + A.v.z * B.v.z;
}


bool LOTRI_LoadV3M4(const int N, Vec3f vec_in[N], const Mat4f mat, Vec4f vec_out[N], const bool w) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < 4; j++) {
            vec_out[i].arr[j] = 0;
            for (int k = 0; k < 3; k++) {
                vec_out[i].arr[j] += vec_in[i].arr[k] * mat.m[k][j];
            }
            if (w) {
                vec_out[i].arr[j] += mat.m[3][j];
            }
        }
    }
    return true;
}