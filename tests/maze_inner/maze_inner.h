#ifndef MAZE_H
#define MAZE_H


#include "action/action.h"


void MAZE_INNER_Init(SDL_Renderer* renderer);
void MAZE_INNER_Deinit();


void MAZE_INNER_Load();
void MAZE_INNER_Unload();


void MAZE_INNER_Renew();
void MAZE_INNER_Draw();


#endif //MAZE_H