#ifndef VILLA_CHARACTER_H
#define VILLA_CHARACTER_H


#include "villa.h"
#include "room.h"


typedef struct Character Character;


void *VILLA_CreateCharacter(const cJSON *character_json);
void VILLA_DestroyCharacter_V(void *character);
void VILLA_DestroyCharacter(Character *character);
bool VILLA_RenewCharacter(void *character);
bool VILLA_DrawCharacter(const void *character_void);


#endif //VILLA_CHARACTER_H