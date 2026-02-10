#ifndef FILE_H
#define FILE_H


#include "../type.h"


typedef struct {
    SDL_Texture* texture;
    TrigFunc func;
    char* para_string;
} TEMPO_TypeFile;


bool createFile(void* info, const cJSON* info_json);
SDL_Texture* textureFile(void *info);
bool trigFile(void *info, SDL_FRect dst_rect);
void deleteFile(void* info);


#endif //FILE_H