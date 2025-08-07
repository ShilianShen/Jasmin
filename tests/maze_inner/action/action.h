#ifndef ACTION_H
#define ACTION_H



#include "../monster/monster.h"

typedef struct Action Action;
struct Action {
    char* name;
    Item* itemCondition;
    Gene* geneCondition;
};
enum ActionType {
    ACTION_TYPE_PASS,
    NUM_ACTION_TYPES,
};
extern Action* actionSet;
extern int lenActionSet;



void ACTION_Load();
void ACTION_Unload();
void ACTION_Move(Monster* monster, Direction direction);


#endif //ACTION_H