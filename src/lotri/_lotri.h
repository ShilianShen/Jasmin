#ifndef _LOTRI_H
#define _LOTRI_H


#include "../perph/perph.h"


typedef struct {Vec3f xyz; Vec4f rgba; Vec2f uv;} LOTRI_Vertex;
typedef struct {Vec3i ijk; Vec3f xyz;} LOTRI_Face;


bool LOTRI_Init();
bool LOTRI_Renew();
bool LOTRI_Draw();
void LOTRI_Exit();


#endif //_LOTRI_H