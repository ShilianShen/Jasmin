#ifndef BASIC_FUNC_H
#define BASIC_FUNC_H


#include "_basic.h"


// 2D-ARRAY ============================================================================================================
void** allocate2DArray(size_t w, size_t h, size_t elementSize);
void free2DArray(void** array, size_t w);


// SDL =================================================================================================================
char* SDL_GetStringFromSDLColor(SDL_Color color);
char* SDL_GetStringFromFRect(SDL_FRect rect);
bool SDL_CompareSDLColor(SDL_Color x, SDL_Color y);
bool SDL_ReadSurfaceSDLColor(SDL_Surface* surface, int x, int y, SDL_Color* color);
bool SDL_SetRenderSDLColor(SDL_Renderer* renderer, SDL_Color color);
SDL_Texture* TXT_LoadTexture(
    SDL_Renderer* renderer,
    TTF_Font* font,
    const char* text,
    SDL_Color color
    );
SDL_Texture* TXT_LoadTextureWithLines(
    SDL_Renderer* renderer,
    TTF_Font* font,
    const char* scr_text,
    SDL_Color colorText,
    SDL_Color colorBack,
    char aligned
    );
bool SDL_LoadDstRectAligned(
    SDL_FRect *dst_rect,
    SDL_Texture *texture,
    const SDL_FRect *src_rect,
    const SDL_FRect *gid_rect,
    const SDL_FRect *bck_rect,
    int anchor
    );
bool SDL_RenderTextureAligned(
    SDL_Renderer* renderer,
    SDL_Texture* texture,
    const SDL_FRect* src_rect,
    const SDL_FRect* gid_rect,
    const SDL_FRect* bck_rect,
    int anchor
    );


// JSON ================================================================================================================
cJSON* getJson(const char* path);
bool cJSON_ExistKey(const cJSON* object, const char* key);
bool cJSON_LoadFromObj(const cJSON* object, const char* key, JSM_DATA_TYPE type, void* target);


#endif //BASIC_FUNC_H