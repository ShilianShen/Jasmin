#ifndef TEMPO_BASIC_H
#define TEMPO_BASIC_H


#include "tempo.h"


typedef enum {
    TEMPO_TEXT_TYPE_TEXT,
    TEMPO_TEXT_TYPE_HEAD_1,
    TEMPO_TEXT_TYPE_HEAD_2,
    TEMPO_TEXT_TYPE_HEAD_3,
    TEMPO_TEXT_TYPE_HEAD_4,
    TEMPO_TEXT_NUM_TYPES,
} TEMPO_TEXT_TYPE;


typedef struct Theme Theme;
extern struct Theme {
    TTF_Font* fonts[TEMPO_TEXT_NUM_TYPES];
    SDL_Color colors[TEMPO_TEXT_NUM_TYPES];
    Table fontTable;
} theme;


bool TEMPO_LoadTheme();
void TEMPO_UnloadTheme();


#endif //TEMPO_BASIC_H