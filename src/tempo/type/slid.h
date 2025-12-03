#ifndef SLID_H
#define SLID_H


#include "../type.h"


typedef struct {
    bool readonly, discrete;
    float min, max, *now;
    SDL_Texture* texture;
    SDL_FPoint point;
} TypeSlid;
bool TEMPO_CreateTypeSlid(void* info, const cJSON* info_json);
bool TEMPO_RenewTypeSlid(void* info, SDL_Texture** tex, const SDL_FPoint *mouseL, const SDL_FPoint *mouseR);
void TEMPO_DeleteTypeSlid(void* info);


#endif //SLID_H