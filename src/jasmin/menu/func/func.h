#ifndef JASMIN_MENU_FUNC_H
#define JASMIN_MENU_FUNC_H


#include "interface.h"


void MENU_Init(SDL_Renderer* renderer);
void MENU_Deinit();


void MENU_Load(const char* menuPagesPath, const char* menuThemePath);
void MENU_Unload();


void MENU_Renew();
void MENU_Draw();


#endif //JASMIN_MENU_FUNC_H