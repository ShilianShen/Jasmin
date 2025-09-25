#ifndef VILLA_CHARACTER_H
#define VILLA_CHARACTER_H


#include "villa.h"
#include "room.h"


typedef enum {
    VILLA_DIRECT_W,
    VILLA_DIRECT_S,
    VILLA_DIRECT_A,
    VILLA_DIRECT_D,
    VILLA_NUM_DIRECTS,
} VILLA_Direct;
typedef enum {
    VILLA_ACT_NONE,
    VILLA_ACT_WALK_1,
    VILLA_ACT_WALK_2,
    VILLA_ACT_MOVE_aa,
    VILLA_NUM_ACTS,
} VILLA_Action;
extern VILLA_Direct cameraDirect;


extern SDL_FRect TEX_SRC[VILLA_NUM_DIRECTS][VILLA_NUM_ACTS];
typedef struct Character Character;

bool VILLA_SetCharacterCoord(Character* character, Coord coord);
bool VILLA_SetCharacterMove(Character* character, VILLA_Direct direct);
void *VILLA_CreateCharacter(const cJSON *character_json);

void *VILLA_DeleteCharacter(void *character_void);
bool VILLA_RenewCharacter(void *character_void);
bool VILLA_DrawCharacter(const void *character_void);


#endif //VILLA_CHARACTER_H