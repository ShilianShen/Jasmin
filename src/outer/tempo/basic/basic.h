#ifndef TEMPO_BASIC_H
#define TEMPO_BASIC_H


#include "../interface.h"


typedef struct Basic Basic;
extern struct Basic {
    SDL_Renderer* renderer;
    TTF_Font* font; // malloc
} basic;


void TEMPO_SetBasicRenderer(SDL_Renderer*);
void TEMPO_SetBasicFont(TTF_Font*);


bool TEMPO_LoadBasic();
void TEMPO_UnloadBasic();


extern KeyVal TEMPO_MENU_TRIG_SET[];
void TEMPO_TrigFuncPass(const char*);
void TEMPO_TrigFuncForward(const char*);
void TEMPO_TrigFuncBackward(const char*);
void TEMPO_TrigFuncClear(const char*);
void TEMPO_TrigFuncSwitch(const char*);


#endif //TEMPO_BASIC_H