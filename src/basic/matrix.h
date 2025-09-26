#ifndef BASIC_MATRIX_H
#define BASIC_MATRIX_H


#include "_basic.h"


typedef union {struct {float x, y, z, w;} v; float arr[4];} Vec4f;
typedef union {struct {float x, y, z;} v; float arr[3];} Vec3f;
typedef union {struct {float x, y;} v; float arr[2];} Vec2f;
typedef union {struct {int i, j, k;} v; int arr[3];} Vec3i;


Vec3f BASIC_GetNormal(Vec3f A, Vec3f B, Vec3f C);
Vec3f BASIC_GetSum(Vec3f A, Vec3f B, Vec3f C);
float BASIC_GetDot(Vec3f A, Vec3f B);
Vec3f BASIC_GetAdd(Vec3f A, Vec3f B);
const char* BASIC_GetStrVec3f(Vec3f vec);
bool BASIC_GetVecEqual(Vec3f A, Vec3f B);


typedef struct {float m[4][4];} Mat4f;


Mat4f BASIC_GetMatXMat(Mat4f A, Mat4f B);
Mat4f BASIC_GetProd(int N, const Mat4f matArray[]);
Mat4f BASIC_GetMatR(Vec3f vec);
Mat4f BASIC_GetInvR(Vec3f vec);
Mat4f BASIC_GetMatS(Vec3f vec);
Mat4f BASIC_GetMatT(Vec3f vec);
Mat4f BASIC_GetInvT(Vec3f vec);


#endif //BASIC_MATRIX_H