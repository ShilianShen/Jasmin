#ifndef ELEM_TEXT_H
#define ELEM_TEXT_H


#include "../tempo.h"


typedef struct {TTF_Font* font; bool key; char* string; SDL_Color backColor;} TypeTextInfo;


bool TEMPO_CreateTypeText(void* info, const cJSON* info_json);
bool TEMPO_RenewTypeText(const void* info, SDL_Texture** tex);
void TEMPO_DeleteTypeText(void* info);


#endif //ELEM_TEXT_H