#ifndef JASMIN_LOTRI_MATH_MAT_H
#define JASMIN_LOTRI_MATH_MAT_H


#include "../vec/vec.h"


typedef struct Mat4x4f {float m[4][4];} Mat4x4f;


extern const Mat4x4f unitMat;


Mat4x4f MAT_GetProduct(Mat4x4f, Mat4x4f);
void MAT_LoadProductVecMat(int N, bool w, Vec3f vecOut[N], const Vec3f vecIn[N], Mat4x4f mat);


Mat4x4f MAT_GetMatSRT(const Vec3f*, const Vec3f*, const Vec3f*);
Mat4x4f MAT_GetInvSRT(const Vec3f* scale, const Vec3f* rotation, const Vec3f* translation);


#endif //JASMIN_LOTRI_MATH_MAT_H