#ifndef TEMPO_TYPE_H
#define TEMPO_TYPE_H


#include "tempo.h"


extern Table TEMPO_TrigFuncTable;


typedef struct TEMPO_Type TEMPO_Type;
TEMPO_Type* TEMPO_CreateType(const cJSON* type_json);
TEMPO_Type* TEMPO_DeleteType(TEMPO_Type* type);
SDL_Texture* TEMPO_RenewTypeTexture(TEMPO_Type* type);
bool TEMPO_RenewTypeTrig(TEMPO_Type* type, SDL_FRect dst_rect);


#endif //TEMPO_TYPE_H