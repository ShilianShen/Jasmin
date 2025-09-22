#ifndef VILLA_CHARACTER_H
#define VILLA_CHARACTER_H


#include "villa.h"
#include "room.h"


typedef enum VILLA_Direct {
    DIRECT_W,
    DIRECT_A,
    DIRECT_S,
    DIRECT_D,
    NUM_DIRECTS,
} VILLA_Direct;


typedef struct Character Character;

bool VILLA_SetCharacterPosition(Character* character, const Room* room, int x, int y);
bool VILLA_SetCharacterMove(Character* character, VILLA_Direct direct);
void *VILLA_CreateCharacter(const cJSON *character_json);
void VILLA_DeleteCharacter(void *character_void);
bool VILLA_RenewCharacter(void *character_void);
bool VILLA_DrawCharacter(const void *character_void);


#endif //VILLA_CHARACTER_H