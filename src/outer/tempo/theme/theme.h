#ifndef TEMPO_BASIC_H
#define TEMPO_BASIC_H


#include "../interface.h"


typedef struct Theme Theme;
extern struct Theme {
    TTF_Font* font; // malloc
} theme;


bool TEMPO_LoadTheme();
void TEMPO_UnloadTheme();


#endif //TEMPO_BASIC_H