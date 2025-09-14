#include "matrix.h"


Matrix LOTRI_MultipleMatrix(const Matrix A, const Matrix B) {
    Matrix AB = {0};
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                AB.m[i][j] += A.m[i][k] * B.m[k][j];
            }
        }
    }
    return AB;
}
Matrix LOTRI_LoadR(const float x, const float y, const float z) {
    const Matrix rz = {
        .m = {
            {SDL_cosf(z), -SDL_sinf(z), 0, 0},
            {SDL_sinf(z), SDL_cosf(z), 0, 0},
            {0, 0, 1, 0},
            {0, 0, 0, 1}
        }
    };
    const Matrix ry = {
        .m = {
            {SDL_cosf(y), 0, -SDL_sinf(y), 0},
            {0, 1, 0, 0},
            {SDL_sinf(y), 0, SDL_cosf(y), 0},
            {0, 0, 0, 1}
        }
    };
    const Matrix rx = {
        .m = {
            {1, 0, 0, 0},
            {0, SDL_cosf(x), -SDL_sinf(x), 0},
            {0, SDL_sinf(x), SDL_cosf(x), 0},
            {0, 0, 0, 1}
        }
    };
    const Matrix rzy = LOTRI_MultipleMatrix(rz, ry);
    return LOTRI_MultipleMatrix(rzy, rx);

}
Matrix LOTRI_LoadS(const float x, const float y, const float z) {
    const Matrix result = {
        {
            {x, 0, 0, 0},
            {0, y, 0, 0},
            {0, 0, z, 0},
            {0, 0, 0, 1}
        }
    };
    return result;

}


bool LOTRI_Trans(const int N, vec4f vec_in[N], const Matrix mat, vec4f vec_out[N]) {
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