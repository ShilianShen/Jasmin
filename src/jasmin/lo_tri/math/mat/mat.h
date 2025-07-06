#ifndef JASMIN_LOTRI_MATH_MAT_H
#define JASMIN_LOTRI_MATH_MAT_H


#include "../vec/vec.h"


typedef struct Mat4x4f {float m[4][4];} Mat4x4f;


extern const Mat4x4f unitMat;


Mat4x4f MAT_GetProduct(Mat4x4f, Mat4x4f);


Mat4x4f MAT_GetTranslation(Vec3f);
Mat4x4f MAT_GetRotation(Vec3f);
Mat4x4f MAT_GetScale(Vec3f);
Mat4x4f MAT_GetTRS(Vec3f, Vec3f, Vec3f);


#endif //JASMIN_LOTRI_MATH_MAT_H