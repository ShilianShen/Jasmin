#ifndef ELEM_TEXT_H
#define ELEM_TEXT_H


#include "../elem.h"


typedef struct ElemTextInfo {TTF_Font* font; bool key; char* string;} ElemTextInfo;


bool TEMPO_CreateElemText(void* info, const cJSON* info_json);
bool TEMPO_RenewElemText(const void* info, SDL_Texture** tex);
void TEMPO_DeleteElemText(void* info);


#endif //ELEM_TEXT_H