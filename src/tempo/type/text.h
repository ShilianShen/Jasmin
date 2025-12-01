#ifndef ELEM_TEXT_H
#define ELEM_TEXT_H


#include "../tempo.h"


typedef struct {TTF_Font* font; bool key; char* string; SDL_Color backColor;} TypeText;


bool TEMPO_CreateTypeText(void* info, const cJSON* info_json);
bool TEMPO_RenewTypeText(void *info, SDL_Texture** tex, const SDL_FPoint *point);
void TEMPO_DeleteTypeText(void* info);


#endif //ELEM_TEXT_H