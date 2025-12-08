#ifndef MANU_H
#define MANU_H


#include "../tempo.h"


bool createManu(void* info, const cJSON* info_json);
SDL_Texture* textureManu(void* info);
bool trigManu(void* info, SDL_FPoint mouse, SDL_FRect dst_rect);
void deleteManu(void* info);


#endif //MANU_H