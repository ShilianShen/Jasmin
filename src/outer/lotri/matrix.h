#ifndef MATRIX_H
#define MATRIX_H


#include "_lotri.h"




// VEC =================================================================================================================
typedef union {struct {float x, y, z, w;} v; float arr[4];} Vec4f;
typedef union {struct {float x, y, z;} v; float arr[3];} Vec3f;
typedef union {struct {float x, y;} v; float arr[2];} Vec2f;
typedef union {struct {int i, j, k;} v; int arr[3];} Vec3i;


Vec3f LOTRI_GetNormal(Vec3f A, Vec3f B, Vec3f C);
Vec3f LOTRI_GetSum(Vec3f A, Vec3f B, Vec3f C);
float LOTRI_GetDot(Vec3f A, Vec3f B);


// MAT =================================================================================================================
typedef struct {float m[4][4];} Mat4f;


Mat4f LOTRI_GetMatXMat(Mat4f A, Mat4f B);
Mat4f LOTRI_GetProd(int N, const Mat4f matArray[]);
Mat4f LOTRI_GetMatR(Vec3f vec);
Mat4f LOTRI_GetInvR(Vec3f vec);
Mat4f LOTRI_GetMatS(Vec3f vec);
Mat4f LOTRI_GetMatT(Vec3f vec);
Mat4f LOTRI_GetInvT(Vec3f vec);


// VERTEX & FACE =======================================================================================================
typedef struct {Vec3f xyz; Vec4f rgba; Vec2f uv;} LOTRI_Vertex;
typedef struct {Vec3i ijk; Vec3f xyz;} LOTRI_Face;


bool LOTRI_LoadV3M4(int N, Vec3f vec_in[N], Mat4f mat, Vec4f vec_out[N], bool w);
Vec3f LOTRI_AtvVec(Vec3f a, Vec3f b, float t, AtvFunc atv);


#endif //MATRIX_H