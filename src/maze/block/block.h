#ifndef MAZE_BLOCK_H
#define MAZE_BLOCK_H


#include "../monster/monster.h"


typedef struct Birth {
    int id;
    int x, y;
} Birth;


typedef struct Block {
    SDL_Color color;

    int w, h;
    int** wall; // malloc in LOAD

    int numBirth;
    Birth* birth; // ?

    SDL_Texture* texture; // ?
} Block;


extern int lenBlockSet;
extern Block* blockSet;


void BLOCK_Load();
void BLOCK_Unload();


void BLOCK_Draw();


#endif //MAZE_BLOCK_H