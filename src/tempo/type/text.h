#ifndef ELEM_TEXT_H
#define ELEM_TEXT_H


#include "../type.h"


typedef struct {TTF_Font* font; bool key; char* string; SDL_Color backColor; TrigFunc func; char* para_string;} TypeText;
bool TEMPO_CreateTypeText(void* info, const cJSON* info_json);
bool TEMPO_RenewTypeText(void *info, SDL_Texture** tex, const SDL_FPoint *mouseL, const SDL_FPoint *mouseR);
void TEMPO_DeleteTypeText(void* info);


#endif //ELEM_TEXT_H