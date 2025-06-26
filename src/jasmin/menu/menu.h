#ifndef JASMIN_MENU_H
#define JASMIN_MENU_H


#include "../basic/basic.h"
#include "struct/menu.h"
#include "func/func.h"


void MENU_Draw();
void MENU_Init(SDL_Renderer*);
void MENU_Kill();
void MENU_Load(const char*, const char*);
void MENU_Renew();


#endif //JASMIN_MENU_H