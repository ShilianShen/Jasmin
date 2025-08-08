#ifndef JASMIN_BASIC_DEBUG_H
#define JASMIN_BASIC_DEBUG_H


/*
 * Debug是同来简化Debug的, 尤其是显示的部分.
 * 所以Debug函数在Debug.on = false时都不会做任何事情.
 */


#include "../basic/basic.h"


void DEBUG_Init(SDL_Renderer* renderer);
void DEBUG_Load();
void DEBUG_Renew();


void DEBUG_Intro();


void DEBUG_DrawPoint(Sint16 x, Sint16 y);
void DEBUG_DrawLine(float x1, float y1, float x2, float y2);
void DEBUG_DrawRect(const SDL_FRect* rect);
void DEBUG_FillRect(const SDL_FRect* rect);
void DEBUG_DrawText(Sint16 x, Sint16 y, const char* text);
void DEBUG_DrawTextAligned(const char* text, char aligned);
void DEBUG_DrawFace(const SDL_Vertex* vertices);


void DEBUG_SendMessageL(const char* format, ...);
void DEBUG_SendMessageR(const char* format, ...);
void DEBUG_DrawMessage();


#endif //JASMIN_BASIC_DEBUG_H