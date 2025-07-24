#ifndef MAZE_MONSTER_H
#define MAZE_MONSTER_H


#include "../block/block.h"


typedef struct Position {
    Block* block;
    int x, y;
} Position;


typedef struct Monster {
    Gene* gene;
    Position posBirth;
    Position pos;
    int life;
    int lifeBuffer;
} Monster;


extern Monster* monsterSet;


void MONSTER_Load();
void MONSTER_Unload();


void MONSTER_Renew();
void MONSTER_Draw();


#endif //MAZE_MONSTER_H