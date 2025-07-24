#ifndef MAZE_BLOCK_H
#define MAZE_BLOCK_H


#include "../gene/gene.h"




typedef struct Room {
    SDL_Color color;
    SDL_Color gateColors[NUM_DIRECTIONS];
    SDL_Surface* surface;

    int w, h;
    bool** wall; // malloc in LOAD

    Elem* elem; // malloc
} Room;




extern int lenRoomSet;
extern Room* roomSet;
extern Direction** roomNet;


void ROOM_Load();
void ROOM_Unload();


void ROOM_Renew();
void ROOM_Draw_BDI();


#endif //MAZE_BLOCK_H