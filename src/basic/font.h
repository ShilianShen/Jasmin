#ifndef FONT_H
#define FONT_H


#include "_basic.h"
#include "table.h"


bool BASIC_InitFont();
void BASIC_ExitFont();
TTF_Font* BASIC_GetFont(const char* key);


#endif //FONT_H