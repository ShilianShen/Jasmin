#ifndef MAZE_BLOCK_H
#define MAZE_BLOCK_H


#include "../gene/gene.h"


#define SET_ILLEGAL_INDEX -1


typedef struct Room {
    SDL_Color color;
    char* path;
    SDL_Color gateColors[NUM_DIRECTIONS];
    SDL_Surface* surface;
    int w, h;
    bool** wall; // malloc in LOAD
} Room;


typedef struct Position {
    Room* room;
    int x, y;
    Direction direction;
} Position;


extern int lenRoomSet;
extern Room* roomSet;
extern Direction** roomNet;


void ROOM_Load();
void ROOM_Unload();
void ROOM_Renew();


#endif //MAZE_BLOCK_H