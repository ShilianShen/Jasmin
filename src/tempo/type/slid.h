#ifndef SLID_H
#define SLID_H

#include "../tempo.h"


typedef struct {bool readonly; bool discrete; float min, max, *now;} TypeSlidInfo;


bool TEMPO_CreateTypeSlid(void* info, const cJSON* info_json);
bool TEMPO_RenewTypeSlid(const void* info, SDL_Texture** tex);
void TrigFunc_Slid(const TypeSlidInfo* slid, SDL_FRect dst_rect);
void TEMPO_TrigFuncSlid(TrigPara para);

#endif //SLID_H