#ifndef JASMIN_BASIC_DEBUG_H
#define JASMIN_BASIC_DEBUG_H


#include "../ease/ease.h"


void DEBUG_Init(SDL_Renderer* renderer);
void DEBUG_Load();
void DEBUG_Renew();


void DEBUG_Intro();


void DEBUG_DrawPoint(const Sint16 x, const Sint16 y);
void DEBUG_DrawLine(const float x1, const float y1, const float x2, const float y2);
void DEBUG_DrawRect(const SDL_FRect* rect);
void DEBUG_FillRect(const SDL_FRect* rect);
void DEBUG_DrawText(const Sint16 x, const Sint16 y, const char* text);


void DEBUG_SendMessage(const char* format, ...);
bool DEBUG_HaveMessage();
void DEBUG_DrawMessage(const Sint16 x, const Sint16 y);


#endif //JASMIN_BASIC_DEBUG_H