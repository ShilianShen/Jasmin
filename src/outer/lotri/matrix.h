#ifndef MATRIX_H
#define MATRIX_H


#include "lotri.h"


typedef struct {float m[4][4];} Mat4f;
typedef union {struct {float x, y, z, w;} v; float arr[4];} Vec4f;
typedef union {struct {float x, y, z;} v; float arr[3];} Vec3f;
typedef union {struct {int a, b, c;} v; int arr[3];} Vec3i;


Mat4f LOTRI_GetMatXMat(Mat4f A, Mat4f B);
Mat4f LOTRI_GetProd(int N, Mat4f matArray[N]);
Mat4f LOTRI_GetMatR(Vec3f vec);
Mat4f LOTRI_GetInvR(Vec3f vec);
Mat4f LOTRI_GetMatS(Vec3f vec);

Vec3f LOTRI_GetNormal(Vec3f A, Vec3f B, Vec3f C);
Vec3f LOTRI_GetSum(Vec3f A, Vec3f B, Vec3f C);
float LOTRI_GetDot(Vec3f A, Vec3f B);

bool LOTRI_LoadV3M4(int N, Vec3f vec_in[N], Mat4f mat, Vec4f vec_out[N], bool w);


#endif //MATRIX_H