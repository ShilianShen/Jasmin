#ifndef JASMIN_BASIC_SETTING_H
#define JASMIN_BASIC_SETTING_H


#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <stdlib.h>
#include "../../../../third_party/toml/toml.h"


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