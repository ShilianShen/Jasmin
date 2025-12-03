#ifndef SWITCH_H
#define SWITCH_H


#include "../tempo.h"


typedef struct {bool readonly; bool* now; SDL_Texture* texture;} TypeBool;
bool TEMPO_CreateTypeBool(void* info, const cJSON* info_json);
bool TEMPO_RenewTypeBool(void *info, SDL_Texture** tex, const SDL_FPoint *mouseL, const SDL_FPoint *mouseR);
void TEMPO_DeleteTypeBool(void *info);


#endif //SWITCH_H