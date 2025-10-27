#ifndef MESSAGE_H
#define MESSAGE_H


#include "villa.h"


bool VILLA_SendMessage(const char *name, const char* string);

bool VILLA_ClearMessage();
bool VILLA_HaveMessage();


bool VILLA_InitMessage();
bool VILLA_RenewMessage();
bool VILLA_DrawMessage();
void VILLA_ExitMessage();


#endif //MESSAGE_H