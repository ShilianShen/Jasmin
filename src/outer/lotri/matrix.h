#ifndef MATRIX_H
#define MATRIX_H


#include "lotri.h"


typedef struct Matrix {
    float m[4][4];
} Matrix;


typedef union vec4f {
    struct {float x, y, z, a;} v;
    float arr[4];
} vec4f;


typedef union vec4i {
    struct {int a, b, c, d;} v;
    int arr[4];
} vec4i;


Matrix LOTRI_MultipleMatrix(Matrix A, Matrix B);
Matrix LOTRI_LoadR(float x, float y, float z);
Matrix LOTRI_LoadS(float x, float y, float z);
bool LOTRI_Trans(int N, vec4f vec_in[N], Matrix mat, vec4f vec_out[N]);


#endif //MATRIX_H