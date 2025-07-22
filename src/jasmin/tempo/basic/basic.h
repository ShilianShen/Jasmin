#ifndef TEMPO_BASIC_H
#define TEMPO_BASIC_H


#include "../interface.h"


extern SDL_Renderer* tempoRenderer;
extern TTF_Font* tempoTextFont;
extern SDL_FRect tempoBckRect;


bool TEMPO_GetBasicOk();
void TEMPO_SetBasicRenderer(SDL_Renderer*);
void TEMPO_SetBasicFont(TTF_Font*);
void TEMPO_SetBasicBckRect(SDL_FRect);


#endif //TEMPO_BASIC_H