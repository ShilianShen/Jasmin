#ifndef COORD_H
#define COORD_H


#include "../room/room.h"


typedef struct Coord {
    Room* room;
    int x, y;
    Direction direction;
} Coord;


void COORD_RenewCoord(Coord* coord);


#endif //COORD_H