#ifndef FILE_H
#define FILE_H


#include "../type.h"


typedef struct {SDL_Texture* texture; TrigFunc func; char* para_string;} TypeFile;
bool createFile(void* info, const cJSON* info_json);
SDL_Texture* textureFile(void *info);
bool trigFile(void *info, SDL_FPoint mouse);
void deleteFile(void* info);


#endif //FILE_H