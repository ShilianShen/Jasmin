#ifndef MAZE_INNER_ITEM_H
#define MAZE_INNER_ITEM_H


#include "../basic/basic.h"


typedef struct Item Item;
struct Item {
    char* name;
};
extern Item* itemSet;
extern int lenItemSet;


void ITEM_Load();
void ITEM_Unload();


#endif // MAZE_INNER_ITEM_H