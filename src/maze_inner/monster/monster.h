#ifndef MAZE_MONSTER_H
#define MAZE_MONSTER_H


#include "../room/room.h"
#include "../gene/gene.h"
#include "../item/item.h"
#include "../coord/coord.h"


typedef struct Monster Monster;
struct Monster {
    Gene* gene;
    int life;
    int lifeBuffer;
    Coord now;
    Coord start;
    Monster* target;
    Monster* address;
    Item* item;
};
extern Monster* you;

extern Monster* monsterSet;


void MONSTER_Load();
void MONSTER_Unload();


void MONSTER_Renew();


void MONSTER_Move(Monster* monster, Direction direction);


#endif //MAZE_MONSTER_H