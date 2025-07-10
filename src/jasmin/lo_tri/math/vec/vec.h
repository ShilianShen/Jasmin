#ifndef JASMIN_LOTRI_MATH_VEC_H
#define JASMIN_LOTRI_MATH_VEC_H


#include "../interface.h"


typedef union Vec3i {  int v[3]; struct {  int a, b, c   ;};} Vec3i;
typedef union Vec3f {float v[3]; struct {float x, y, z   ;};} Vec3f;
typedef union Vec4f {float v[4]; struct {float x, y, z, w;};} Vec4f;


Vec3f VEC_GetNormal(Vec3f, Vec3f, Vec3f);
Vec3f VEC_GetNegative(Vec3f);
Vec3f VEC_GetReciprocal(Vec3f);
float VEC_GetMean(Vec3f);


#endif //JASMIN_LOTRI_MATH_VEC_H