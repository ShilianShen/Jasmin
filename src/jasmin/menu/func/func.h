#ifndef JASMIN_MENU_FUNC_H
#define JASMIN_MENU_FUNC_H


#include "interface.h"


void MENU_Draw();
void MENU_Init(SDL_Renderer* renderer);
void MENU_Kill();
void MENU_Load(const char* menuPagesPath, const char* menuThemePath);
void MENU_Renew();


#endif //JASMIN_MENU_FUNC_H