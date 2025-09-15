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
Mat4f LOTRI_GetMatR(const float x, const float y, const float z) {
    const Mat4f rz = {
        .m = {
            {SDL_cosf(z), -SDL_sinf(z), 0, 0},
            {SDL_sinf(z), SDL_cosf(z), 0, 0},
            {0, 0, 1, 0},
            {0, 0, 0, 1}
        }
    };
    const Mat4f ry = {
        .m = {
            {SDL_cosf(y), 0, -SDL_sinf(y), 0},
            {0, 1, 0, 0},
            {SDL_sinf(y), 0, SDL_cosf(y), 0},
            {0, 0, 0, 1}
        }
    };
    const Mat4f rx = {
        .m = {
            {1, 0, 0, 0},
            {0, SDL_cosf(x), -SDL_sinf(x), 0},
            {0, SDL_sinf(x), SDL_cosf(x), 0},
            {0, 0, 0, 1}
        }
    };
    const Mat4f rzy = LOTRI_GetMatXMat(rz, ry);
    return LOTRI_GetMatXMat(rzy, rx);

}
Mat4f LOTRI_GetInvR(const float x, const float y, const float z) {
    const Mat4f rz = {
        .m = {
            {SDL_cosf(-z), -SDL_sinf(-z), 0, 0},
            {SDL_sinf(-z), SDL_cosf(-z), 0, 0},
            {0, 0, 1, 0},
            {0, 0, 0, 1}
        }
    };
    const Mat4f ry = {
        .m = {
            {SDL_cosf(-y), 0, -SDL_sinf(-y), 0},
            {0, 1, 0, 0},
            {SDL_sinf(-y), 0, SDL_cosf(-y), 0},
            {0, 0, 0, 1}
        }
    };
    const Mat4f rx = {
        .m = {
            {1, 0, 0, 0},
            {0, SDL_cosf(-x), -SDL_sinf(-x), 0},
            {0, SDL_sinf(-x), SDL_cosf(-x), 0},
            {0, 0, 0, 1}
        }
    };
    const Mat4f yz = LOTRI_GetMatXMat(ry, rz);
    return LOTRI_GetMatXMat(rx, yz);

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