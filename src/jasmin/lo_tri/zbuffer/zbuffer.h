#ifndef ZBUFFER_H
#define ZBUFFER_H


#include "../pipline/pipline.h"


typedef struct ZBuffer {
    int numFaces;
    int head;
    Face** faces; // malloc, Face*[]
    float* zs; // malloc
    int* indices; // malloc
} ZBuffer;
extern ZBuffer zBuffer;


void ZBUFFER_Init();
void ZBUFFER_Deinit();

void ZBUFFER_Load();
void ZBUFFER_Unload();


void ZBUFFER_Renew();
void ZBUFFER_DrawModel(const Model model);
void ZBUFFER_Draw();


#endif //ZBUFFER_H