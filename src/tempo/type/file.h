#ifndef FILE_H
#define FILE_H


#include "../tempo.h"


typedef struct {char* string;} TypeFileInfo;


bool TEMPO_CreateTypeFile(void* info, const cJSON* info_json);
bool TEMPO_RenewTypeFile(const void* info, SDL_Texture** tex);
void TEMPO_DeleteTypeFile(void* info);


#endif //FILE_H