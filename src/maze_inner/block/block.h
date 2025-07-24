#ifndef MAZE_BLOCK_H
#define MAZE_BLOCK_H


#include "../gene/gene.h"




typedef struct Block {
    SDL_Color color;
    SDL_Color gateColors[NUM_DIRECTIONS];
    SDL_Surface* surface;

    int w, h;
    bool** wall; // malloc in LOAD

    Elem* elem; // malloc
} Block;




extern int lenBlockSet;
extern Block* blockSet;
extern Direction** blockNet;


void BLOCK_Load();
void BLOCK_Unload();


void BLOCK_Renew();
void BLOCK_Draw_BDI();


#endif //MAZE_BLOCK_H