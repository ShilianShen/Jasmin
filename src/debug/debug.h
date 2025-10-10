#ifndef JASMIN_DEBUG_H
#define JASMIN_DEBUG_H


#include "../basic/basic.h"


extern const bool DEBUG_ON;


bool DEBUG_Init();
bool DEBUG_Renew();
bool DEBUG_Draw();
void DEBUG_Exit();


bool DEBUG_GetShift();


void DEBUG_DrawPoint(SDL_FPoint point);
void DEBUG_DrawLine(SDL_FPoint point1, SDL_FPoint point2);
void DEBUG_DrawRect(SDL_FRect rect);
void DEBUG_FillRect(SDL_FRect rect);
void DEBUG_DrawText(SDL_FPoint point, const char* text);
void DEBUG_DrawTextAligned(const char* text, char aligned);
void DEBUG_DrawGeometry(
    SDL_Renderer *renderer,
    SDL_Texture *texture,
    const SDL_Vertex *vertices, int num_vertices,
    const int *indices, int num_indices
    );


void DEBUG_SendMessageL(const char* format, ...);
void DEBUG_SendMessageR(const char* format, ...);


#endif //JASMIN_DEBUG_H