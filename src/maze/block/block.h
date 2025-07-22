#ifndef MAZE_BLOCK_H
#define MAZE_BLOCK_H


#include "../monster/monster.h"


typedef struct Birth {
    int id;
    int x, y;
} Birth;

typedef enum BlockGate {
    BLOCK_GATE_W,
    BLOCK_GATE_A,
    BLOCK_GATE_S,
    BLOCK_GATE_D,
    NUM_BLOCK_GATES,
    BLOCK_GATE_ILLEGAL,
} BlockGate;

typedef struct Block {
    SDL_Color color;

    int w, h;
    int** wall; // malloc in LOAD

    int numBirth;
    Birth* birth; // malloc

    Elem* elem; // malloc

    SDL_Color gateColors[NUM_BLOCK_GATES];
    int depth;
    int gateDepths[NUM_BLOCK_GATES];
    SDL_FRect dst_rect;
    SDL_FRect dst_rects[NUM_BLOCK_GATES];
} Block;


extern int lenBlockSet;
extern Block* blockSet;
extern BlockGate** blockNet;


void BLOCK_Load();
void BLOCK_Unload();


void BLOCK_Renew();
void BLOCK_Draw();


#endif //MAZE_BLOCK_H