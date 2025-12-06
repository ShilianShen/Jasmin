#ifndef SWITCH_H
#define SWITCH_H


#include "../tempo.h"


typedef struct {bool readonly; bool* now; SDL_Texture* texture;} TypeBool;
bool createBool(void* info, const cJSON* info_json);
SDL_Texture* textureBool(void* info);
bool trigBool(void* info, SDL_FPoint mouse);
void deleteBool(void *info);


#endif //SWITCH_H