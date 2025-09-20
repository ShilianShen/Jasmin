#ifndef VILLA_CHARACTER_H
#define VILLA_CHARACTER_H


#include "villa.h"
#include "room.h"


typedef struct Character Character;


Character* VILLA_CreateCharacter(const cJSON* character_json);
Character* VILLA_DeleteCharacter(Character* character);
bool VILLA_RenewCharacter(Character *character);
bool VILLA_DrawCharacter(const Character *character);


#endif //VILLA_CHARACTER_H