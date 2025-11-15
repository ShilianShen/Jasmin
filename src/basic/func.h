#ifndef BASIC_FUNC_H
#define BASIC_FUNC_H


#include "_basic.h"


// NORMAL ==============================================================================================================
float clip(float min, float value, float max);
float loop(float min, float value, float max);


// 2D ARRAY ============================================================================================================
void** allocate2DArray(size_t w, size_t h, size_t elementSize);
void free2DArray(void** array, size_t w);


// SDL =================================================================================================================
bool SDL_GetPointInRect(SDL_FPoint point, SDL_FRect rect);
char*SDL_GetStrColor(SDL_Color color);
char*SDL_GetStrFRect(SDL_FRect rect);
char*SDL_GetStrFPoint(SDL_FPoint point);
bool SDL_GetColorEqual(SDL_Color x, SDL_Color y);
bool SDL_RenderParallelLine(SDL_Renderer *renderer, SDL_FPoint A, SDL_FPoint B, float offset);
bool SDL_RenderDashedLine(SDL_Renderer *renderer, SDL_FPoint A, SDL_FPoint B, float dash_len, float gap_len, float offset);
bool SDL_SetTextureColorRGB(SDL_Texture* tex, SDL_Color color);
bool SDL_GetSurfaceColor(SDL_Surface* surface, int x, int y, SDL_Color* color);
bool SDL_SetSurfaceColor(SDL_Surface* surface, int x, int y, SDL_Color color);
SDL_FPoint SDL_GetSumFPoint(int N, const SDL_FPoint points[N]);
bool SDL_SetRenderColor(SDL_Renderer* renderer, SDL_Color color);
SDL_FColor SDL_GetFColorFromColor(SDL_Color color);
SDL_FPoint SDL_ClipPointInRect(SDL_FPoint point, SDL_FRect rect);


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
bool SDL_RenderText(SDL_Renderer* renderer, TTF_Font* font, const char* text, SDL_FPoint point, SDL_Color color);


// OBJ & MTL ===========================================================================================================
typedef struct {
    char name[64];
    char map_Kd[256];
    char map_Bump[256];
    char map_Ks[256];
} MTLMaterial;
int parse_mtl_file(const char* path, MTLMaterial* materials, int max_materials);


// JSON ================================================================================================================
cJSON* getJson(const char* path);
bool cJSON_ExistKey(const cJSON* object, const char* key);
bool cJSON_LoadByKey(const cJSON* object, const char* key, JSM_DataType type, void* target);
bool cJSON_LoadByIdx(const cJSON* object, int idx, JSM_DataType type, void* target);


#endif //BASIC_FUNC_H