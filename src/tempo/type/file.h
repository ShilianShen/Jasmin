#ifndef FILE_H
#define FILE_H


#include "../type.h"


typedef struct {SDL_Texture* texture; TrigFunc func; char* para_string;} TypeFile;
bool TEMPO_CreateTypeFile(void* info, const cJSON* info_json);
bool TEMPO_RenewTypeFile(void *info, SDL_Texture** tex, SDL_FPoint mouse);
void TEMPO_DeleteTypeFile(void* info);


#endif //FILE_H