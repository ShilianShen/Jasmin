#ifndef BASIC_H
#define BASIC_H


#include "../interface.h"


SDL_Renderer* mazeRenderer;


typedef enum Direction {
    DIRECTION_W,
    DIRECTION_A,
    DIRECTION_S,
    DIRECTION_D,
    NUM_DIRECTIONS,
    DIRECTION_ILLEGAL,
} Direction;


#endif //BASIC_H
