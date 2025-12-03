#ifndef MANU_H
#define MANU_H


#include "../tempo.h"


typedef struct {TEMPO_TypeManu* info;} TypeManu;
bool TEMPO_CreateTypeManu(void* info, const cJSON* info_json);
bool TEMPO_RenewTypeManu(void *info, SDL_Texture** texture, const SDL_FPoint* point);
void TEMPO_DeleteTypeManu(void* info);


#endif //MANU_H