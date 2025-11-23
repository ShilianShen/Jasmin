#ifndef SWITCH_H
#define SWITCH_H


#include "../tempo.h"


typedef struct {bool readonly; bool* now;} TypeBoolInfo;


bool TEMPO_CreateTypeBool(void* info, const cJSON* info_json);
bool TEMPO_RenewTypeBool(const void* info, SDL_Texture** tex);
void TEMPO_TrigFuncBool(TrigPara para);


#endif //SWITCH_H