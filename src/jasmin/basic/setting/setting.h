#ifndef JASMIN_BASIC_SETTING_H
#define JASMIN_BASIC_SETTING_H


/*
 * Setting是一些基本函数存放的地方.
 */


#include "../interface.h"


typedef float Pixel;


extern int logical_w, logical_h;
extern int windowWidth, windowHeight;
extern float scale_x, scale_y;


void renewScreenParas(SDL_Window* window);
SDL_Texture* TXT_LoadTexture(SDL_Renderer* renderer, TTF_Font* font, const char* text, SDL_Color color);
SDL_Texture* TXT_LoadTextureWithLines(SDL_Renderer* renderer, TTF_Font* font, const char* scr_text, SDL_Color color, char aligned);
bool SDL_SetRenderSDLColor(SDL_Renderer* renderer, SDL_Color color);
bool SDL_SetRenderSDLColorAlpha(SDL_Renderer* renderer, SDL_Color color, Uint8 alpha);


#endif //JASMIN_BASIC_SETTING_H