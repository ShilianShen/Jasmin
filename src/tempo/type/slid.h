#ifndef SLID_H
#define SLID_H


#include "../type.h"


typedef struct {
    bool readonly, discrete;
    float min, max, *now;
    SDL_Texture* texture;
    SDL_FPoint point;
} TypeSlid;
bool createSlid(void* info, const cJSON* info_json);
SDL_Texture* textureSlid(void* info);
bool trigSlid(void* info, SDL_FPoint mouse);
void deleteSlid(void* info);


#endif //SLID_H