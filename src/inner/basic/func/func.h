#ifndef BASIC_FUNC_H
#define BASIC_FUNC_H


#include "../setting/setting.h"


//
void** allocate2DArray(size_t w, size_t h, size_t elementSize);
void free2DArray(void** array, size_t w);


// SDL扩展
char* SDL_GetStringFromSDLColor(SDL_Color color);
char* SDL_GetStringFromFRect(SDL_FRect rect);
bool SDL_CompareSDLColor(SDL_Color x, SDL_Color y);
bool SDL_ReadSurfaceSDLColor(SDL_Surface* surface, int x, int y, SDL_Color* color);
bool SDL_SetRenderSDLColor(SDL_Renderer* renderer, SDL_Color color);
bool SDL_SetRenderSDLColorAlpha(SDL_Renderer* renderer, SDL_Color color, Uint8 alpha);
SDL_Texture* TXT_LoadTexture(SDL_Renderer* renderer, TTF_Font* font, const char* text, SDL_Color color);
SDL_Texture* TXT_LoadTextureWithLines(SDL_Renderer* renderer, TTF_Font* font, const char* scr_text, SDL_Color colorText, SDL_Color colorBack, char aligned);

bool SDL_LoadDstRectAligned(SDL_FRect *dst_rect, SDL_Texture *texture, const SDL_FRect *src_rect,
                            const SDL_FRect *gid_rect, const SDL_FRect *bck_rect, int anchor);
bool SDL_RenderTextureAligned(SDL_Renderer* renderer, SDL_Texture* texture, const SDL_FRect* src_rect, const SDL_FRect* gid_rect, const SDL_FRect* bck_rect, int anchor);



cJSON* getJson(const char* path);
// toml扩展
toml_table_t* getToml(const char* tomlPath);
bool loadFRectFromTomlArray(SDL_FRect* rect, const toml_array_t* array);


void renewScreenParas(SDL_Window* window);


#endif //BASIC_FUNC_H