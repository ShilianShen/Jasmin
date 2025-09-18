#ifndef JASMIN_BASIC_DEBUG_H
#define JASMIN_BASIC_DEBUG_H


#include "../basic/basic.h"


#define DEBUG_DEFAULT_THEME_JSON "../config/debug_theme.json"
#define DEBUG_ON 1

bool DEBUG_Init();

bool DEBUG_Renew();
void DEBUG_Exit();


void DEBUG_DrawPoint(float x, float y);
void DEBUG_DrawLine(float x1, float y1, float x2, float y2);
void DEBUG_DrawRect(const SDL_FRect* rect);
void DEBUG_FillRect(const SDL_FRect* rect);
void DEBUG_DrawText(Sint16 x, Sint16 y, const char* text);
void DEBUG_DrawTextAligned(const char* text, char aligned);
void DEBUG_DrawGeometry(
    SDL_Renderer *renderer,
    SDL_Texture *texture,
    const SDL_Vertex *vertices, int num_vertices,
    const int *indices, int num_indices);


void DEBUG_SendMessageL(const char* format, ...);
void DEBUG_SendMessageR(const char* format, ...);

bool DEBUG_Draw();


#endif //JASMIN_BASIC_DEBUG_H