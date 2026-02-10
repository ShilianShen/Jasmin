#ifndef ELEM_TEXT_H
#define ELEM_TEXT_H


#include "../type.h"


typedef struct {
    SDL_Texture* texture;
    TTF_Font* font;
    bool key;
    char* string;
    SDL_Color backColor;
    TrigFunc func;
    char* para_string;
} TEMPO_TypeText;
bool createText(void* info, const cJSON* info_json);
SDL_Texture* textureText(void *info);
bool trigText(void *info, SDL_FRect dst_rect);
void deleteText(void* info);


#endif //ELEM_TEXT_H