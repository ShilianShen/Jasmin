#ifndef TEMPO_BASIC_H
#define TEMPO_BASIC_H


#include "../interface.h"


typedef struct Basic Basic;
struct Basic {
    SDL_Renderer* renderer;
    TTF_Font* font; // malloc
    SDL_FRect bck_rect;
};
extern Basic basic;


void TEMPO_SetBasicRenderer(SDL_Renderer*);
void TEMPO_SetBasicFont(TTF_Font*);
void TEMPO_SetBasicBckRect(SDL_FRect);


bool TEMPO_LoadBasic();
void TEMPO_UnloadBasic();


#endif //TEMPO_BASIC_H