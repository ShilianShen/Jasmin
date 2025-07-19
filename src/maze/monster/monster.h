#ifndef MAZE_MONSTER_H
#define MAZE_MONSTER_H


#include "../gene/gene.h"


typedef struct Monster {
    int id;
    Gene* gene;

    int life;
    int lifeBuffer;
} Monster;


extern Monster* monsters;


#endif //MAZE_MONSTER_H