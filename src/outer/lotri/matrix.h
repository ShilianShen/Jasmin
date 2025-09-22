#ifndef MATRIX_H
#define MATRIX_H


#include "_lotri.h"


// VEC =================================================================================================================
typedef struct {float t1, t2; Vec3f v1, v2; AtvFunc func; bool block;} DelayVec3f;
Vec3f LOTRI_GetDelayVecVec(DelayVec3f delay);
bool LOTRI_SetDelayVec(DelayVec3f* delay, Vec3f v2, float time);



// MAT =================================================================================================================



// VERTEX & FACE =======================================================================================================
typedef struct {Vec3f xyz; Vec4f rgba; Vec2f uv;} LOTRI_Vertex;
typedef struct {Vec3i ijk; Vec3f xyz;} LOTRI_Face;


bool LOTRI_LoadV3M4(int N, Vec3f vec_in[N], Mat4f mat, Vec4f vec_out[N], bool w);
Vec3f LOTRI_AtvVec(Vec3f a, Vec3f b, float t, AtvFunc atv);


#endif //MATRIX_H