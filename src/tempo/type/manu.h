#ifndef MANU_H
#define MANU_H


#include "../tempo.h"


typedef struct {TEMPO_ManuElem* elem;} TypeManuInfo;


bool TEMPO_CreateTypeManu(void* info, const cJSON* info_json);
bool TEMPO_RenewTypeManu(const void* info, SDL_Texture** tex);
void TEMPO_DeleteTypeManu(void* info);


#endif //MANU_H