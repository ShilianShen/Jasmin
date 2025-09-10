#ifndef SLID_H
#define SLID_H

#include "../elem.h"


typedef struct ElemSlidInfo {bool readonly; bool discrete; float min, max, *now;} ElemSlidInfo;


bool TEMPO_CreateElemSlid(void* info, const cJSON* info_json);
bool TEMPO_RenewElemSlid(const void* info, SDL_Texture** tex);

#endif //SLID_H