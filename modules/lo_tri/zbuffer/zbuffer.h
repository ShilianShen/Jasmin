#ifndef ZBUFFER_H
#define ZBUFFER_H


#include "../math/math.h"


typedef struct ZBuffer {
    int numFaces;
    int head;
    Face** faces; // malloc, Face*[]
    float* zs; // malloc
    int* indices; // malloc
} ZBuffer;


extern ZBuffer zBuffer;
extern SDL_Renderer* renderer;


void ZBUFFER_Init(SDL_Renderer* ren);
void ZBUFFER_Deinit();


void ZBUFFER_Load();
void ZBUFFER_Unload();


void ZBUFFER_SendFaces(int num, Face faces[num]);
void ZBUFFER_Renew();
void ZBUFFER_Draw();


#endif //ZBUFFER_H