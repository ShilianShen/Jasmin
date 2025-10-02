#ifndef VILLA_CHARACTER_H
#define VILLA_CHARACTER_H


#include "villa.h"
#include "room.h"


extern Table characterTable;
extern int cameraDirect;


extern SDL_FRect TEX_SRC[VILLA_NUM_DIRECTS][VILLA_NUM_ACTS];
typedef struct Character Character;


bool VILLA_GetIfSomeoneThere(Coord coord);
bool VILLA_SetCharacterCoord(Character* character, Coord coord);
bool VILLA_SetCharacterMove(Character* character, int direct);
void *VILLA_CreateCharacter(const cJSON *character_json);

void *VILLA_DeleteCharacter(void *character_void);
bool VILLA_RenewCharacter(void *character_void);
bool VILLA_DrawCharacter(const void *character_void);
bool VILLA_Ask(Character* subject, Character* object);


#endif //VILLA_CHARACTER_H