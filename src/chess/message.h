#ifndef CHESS_MESSAGE_H
#define CHESS_MESSAGE_H


#include "chess.h"


bool VILLA_SendMessage(const char *name, const char* string);
bool VILLA_ClearMessage();
bool VILLA_HaveMessage();
bool VILLA_SetMessageSprite(SDL_Texture* texture);


bool VILLA_InitMessage();
bool VILLA_RenewMessage();
bool VILLA_DrawMessage();
void VILLA_ExitMessage();


#endif //CHESS_MESSAGE_H