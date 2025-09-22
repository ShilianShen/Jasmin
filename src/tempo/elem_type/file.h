#ifndef FILE_H
#define FILE_H


#include "../elem.h"


typedef struct ElemFileInfo {char* string;} ElemFileInfo;


bool TEMPO_CreateElemFile(void* info, const cJSON* info_json);
bool TEMPO_RenewElemFile(const void* info, SDL_Texture** tex);
void TEMPO_DeleteElemFile(void* info);


#endif //FILE_H