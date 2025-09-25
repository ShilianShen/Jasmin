#include "matrix.h"


Vec3f BASIC_GetNormal(const Vec3f A, const Vec3f B, const Vec3f C) {
    const Vec3f AB = {B.v.x - A.v.x, B.v.y - A.v.y, B.v.z - A.v.z};  // 计算 AB 向量
    const Vec3f AC = {C.v.x - A.v.x, C.v.y - A.v.y, C.v.z - A.v.z};  // 计算 AC 向量

    // 计算叉积
    Vec3f normal;
    normal.v.x = AB.v.y * AC.v.z - AB.v.z * AC.v.y;
    normal.v.y = AB.v.z * AC.v.x - AB.v.x * AC.v.z;
    normal.v.z = AB.v.x * AC.v.y - AB.v.y * AC.v.x;

    return normal;
}
Vec3f BASIC_GetSum(const Vec3f A, const Vec3f B, const Vec3f C) {
    const Vec3f result = {
        A.v.x + B.v.x + C.v.x,
        A.v.y + B.v.y + C.v.y,
        A.v.z + B.v.z + C.v.z,
    };
    return result;
}
float BASIC_GetDot(const Vec3f A, const Vec3f B) {
    return A.v.x * B.v.x + A.v.y * B.v.y + A.v.z * B.v.z;
}

bool BASIC_GetVecEqual(const Vec3f A, const Vec3f B) {
    return A.v.x == B.v.x && A.v.y == B.v.y && A.v.z == B.v.z;
}
const char* BASIC_GetStrVec3f(const Vec3f vec) {
    static char string[] = "[9999.99, 9999.99, 9999.99]";
    static size_t len = 0;
    if (len == 0) {
        len = sizeof(string);
    }
    snprintf(string, len, "[%04.2f, %04.2f, %04.2f]", vec.v.x, vec.v.y, vec.v.z);
    return string;

}


Mat4f BASIC_GetMatXMat(const Mat4f A, const Mat4f B) {
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
Mat4f BASIC_GetProd(const int N, const Mat4f matArray[]) {
    Mat4f result = matArray[0];
    for (int i = 1; i < N; i++) {
        result = BASIC_GetMatXMat(result, matArray[i]);
    }
    return result;
}
static Mat4f BASIC_GetMatR_X(const float x) {
    const Mat4f result = {
        {
            {1, 0, 0, 0},
            {0, SDL_cosf(x), SDL_sinf(x), 0},
            {0, -SDL_sinf(x), SDL_cosf(x), 0},
            {0, 0, 0, 1}
        }
    };
    return result;
}
static Mat4f BASIC_GetMatR_Y(const float y) {
    const Mat4f result = {
        {
            {SDL_cosf(y), 0, -SDL_sinf(y), 0},
            {0, 1, 0, 0},
            {SDL_sinf(y), 0, SDL_cosf(y), 0},
            {0, 0, 0, 1}
        }
    };
    return result;
}
static Mat4f BASIC_GetMatR_Z(const float z) {
    const Mat4f result = {
        {
            {SDL_cosf(z), SDL_sinf(z), 0, 0},
            {-SDL_sinf(z), SDL_cosf(z), 0, 0},
            {0, 0, 1, 0},
            {0, 0, 0, 1}
        }
    };
    return result;
}
Mat4f BASIC_GetMatR(const Vec3f vec) {
    const Mat4f M[] = {
        BASIC_GetMatR_X(vec.v.x),
        BASIC_GetMatR_Y(vec.v.y),
        BASIC_GetMatR_Z(vec.v.z),
    };
    return BASIC_GetProd(len_of(M), M);
}
Mat4f BASIC_GetInvR(const Vec3f vec) {
    const Mat4f M[] = {
        BASIC_GetMatR_Z(-vec.v.z),
        BASIC_GetMatR_Y(-vec.v.y),
        BASIC_GetMatR_X(-vec.v.x),
    };
    return BASIC_GetProd(sizeof(M) / sizeof(Mat4f), M);
}
Mat4f BASIC_GetMatS(const Vec3f vec) {
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
Mat4f BASIC_GetMatT(const Vec3f vec) {
    const Mat4f result = {
        .m = {
            {1, 0, 0, 0},
            {0, 1, 0, 0},
            {0, 0, 1, 0},
            {vec.v.x, vec.v.y, vec.v.z, 1},
        }
    };
    return result;
}
Mat4f BASIC_GetInvT(const Vec3f vec) {
    const Mat4f result = {
        .m = {
            {1, 0, 0, 0},
            {0, 1, 0, 0},
            {0, 0, 1, 0},
            {-vec.v.x, -vec.v.y, -vec.v.z, 1},
        }
    };
    return result;
}