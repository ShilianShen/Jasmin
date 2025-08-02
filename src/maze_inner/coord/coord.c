#include "coord.h"


void COORD_RenewCoord(Coord* coord) {
    const Room* room = coord->room;
    if (1 > coord->x) {
        coord->x = 1;
    }
    if (1 > coord->y) {
        coord->y = 1;
    }
    if (coord->x >= room->w - 1) {
        coord->x = room->w - 2;
    }
    if (coord->y >= room->h - 1) {
        coord->y = room->h - 2;
    }
}