#ifndef FILE_H
#define FILE_H


#include "../type.h"


typedef struct {SDL_Texture* texture; TrigFunc func; char* para_string;} TypeFile;
bool TEMPO_CreateTypeFile(void* info, const cJSON* info_json);
bool TEMPO_RenewTypeFile(void *info, SDL_Texture** tex, const SDL_FPoint *mouseL, const SDL_FPoint *mouseR);
void TEMPO_DeleteTypeFile(void* info);


#endif //FILE_H