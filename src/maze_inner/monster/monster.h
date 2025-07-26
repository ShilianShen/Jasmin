#ifndef MAZE_MONSTER_H
#define MAZE_MONSTER_H


#include "../room/room.h"


typedef struct Position {
    Room* block;
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


void MONSTER_Move(Monster* monster, Direction direction);


#endif //MAZE_MONSTER_H