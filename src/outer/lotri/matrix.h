#ifndef MATRIX_H
#define MATRIX_H


#include "lotri.h"


typedef struct {float m[4][4];} Mat4f;
typedef union {struct {float x, y, z, a;} v; float arr[4];} Vec4f;
typedef union {struct {float x, y, z;} v; float arr[3];} Vec3f;
typedef union {struct {int a, b, c;} v; int arr[3];} Vec3i;


Mat4f LOTRI_GetMatXMat(Mat4f A, Mat4f B);
Mat4f LOTRI_GetProd(int N, Mat4f matArray[N]);
Mat4f LOTRI_GetMatR(Vec3f vec);
Mat4f LOTRI_GetInvR(Vec3f vec);
Mat4f LOTRI_GetMatS(Vec3f vec);
bool LOTRI_LoadVecXMat(int N, Vec4f vec_in[N], Mat4f mat, Vec4f vec_out[N]);


#endif //MATRIX_H