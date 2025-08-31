#ifndef TEMPO_BASIC_H
#define TEMPO_BASIC_H


#include "tempo.h"
#define NUM_TEXT_TYPES 4


typedef struct Theme Theme;
extern struct Theme {
    TTF_Font* textFont; // malloc
    TTF_Font* headFonts[NUM_TEXT_TYPES];
    SDL_Color colors[NUM_TEXT_TYPES];
} theme;


bool TEMPO_LoadTheme();
void TEMPO_UnloadTheme();


#endif //TEMPO_BASIC_H