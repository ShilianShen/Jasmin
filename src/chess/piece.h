#ifndef CHESS_PIECE_H
#define CHESS_PIECE_H


#include "chess.h"
#include "board.h"


extern Table characterTable;
extern int cameraDirect;


extern SDL_FRect TEX_SRC[VILLA_NUM_DIRECTS][VILLA_NUM_ACTS];
typedef struct Character Character;


bool VILLA_GetCharacterPosition(const Character* character, Vec3f* position);
bool VILLA_GetIfSomeoneThere(Coord coord);
SDL_Texture* VILLA_GetCharacterSprite(const Character* character);
bool VILLA_SetCharacterCoord(Character* character, Coord coord);
bool VILLA_SetCharacterMove(Character* character, int direct);
const char* VILLA_GetMessageBetween(const Character* subject, const Character* object);
Character* VILLA_GetForwardCharacter(const Character* character);


void *VILLA_CreateCharacter(const cJSON *character_json);
void *VILLA_DeleteCharacter(void *character_void);
bool VILLA_RenewCharacter(void *character_void);
bool VILLA_DrawCharacter(const void *character_void);


#endif //VILLA_CHARACTER_H