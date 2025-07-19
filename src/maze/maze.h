#ifndef MAZE_H
#define MAZE_H


#include "monster/monster.h"


void MAZE_Init(SDL_Renderer* renderer);
void MAZE_Deinit();


void MAZE_Load();
void MAZE_Unload();


void MAZE_Renew();
void MAZE_Draw();


#endif //MAZE_H