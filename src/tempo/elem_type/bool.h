#ifndef SWITCH_H
#define SWITCH_H


#include "../elem.h"


typedef struct ElemBoolInfo {bool readonly; bool* now;} ElemBoolInfo;


bool TEMPO_CreateElemBool(void* info, const cJSON* info_json);
bool TEMPO_RenewElemBool(const void* info, SDL_Texture** tex);


#endif //SWITCH_H