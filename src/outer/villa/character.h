#ifndef CHARACTER_H
#define CHARACTER_H



#include "villa.h"


typedef struct Character Character;


Character* VILLA_CreateCharacter(const cJSON* character);
Character* VILLA_DeleteCharacter(const Character* character);
bool VILLA_RenewCharacter(Character *character);
bool VILLA_DrawCharacter(const Character *character);


#endif //CHARACTER_H