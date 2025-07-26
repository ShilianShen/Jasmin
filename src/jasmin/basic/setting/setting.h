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
extern SDL_Color EMPTY;


void** allocate2DArray(size_t w, size_t h, size_t elementSize);
void free2DArray(void** array, size_t w);
bool loadStringFromSDLColor(char* string, const SDL_Color color);
char* SDL_GetStringFromSDLColor(SDL_Color color);
char* SDL_GetStringFromFRect(SDL_FRect rect);
bool SDL_ReadSurfaceSDLColor(SDL_Surface* surface, int x, int y, SDL_Color* color);
void SDL_PrintFRect(SDL_FRect rect);
toml_table_t* getToml(const char* tomlPath);
void renewScreenParas(SDL_Window* window);
SDL_Texture* TXT_LoadTexture(SDL_Renderer* renderer, TTF_Font* font, const char* text, SDL_Color color);
SDL_Texture* TXT_LoadTextureWithLines(SDL_Renderer* renderer, TTF_Font* font, const char* scr_text, SDL_Color colorText, SDL_Color colorBack, char aligned);
bool SDL_SetRenderSDLColor(SDL_Renderer* renderer, SDL_Color color);
bool SDL_SetRenderSDLColorAlpha(SDL_Renderer* renderer, SDL_Color color, Uint8 alpha);


void SDL_LoadDstRectAligned(SDL_FRect* dst_rect, SDL_Texture* texture, const SDL_FRect* src_rect, const SDL_FRect* gid_rect, const SDL_FRect* bck_rect, int anchor);
bool SDL_RenderTextureAligned(SDL_Renderer* renderer, SDL_Texture* texture, const SDL_FRect* src_rect, const SDL_FRect* gid_rect, const SDL_FRect* bck_rect, int anchor);


#endif //JASMIN_BASIC_SETTING_H