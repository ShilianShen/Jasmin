#ifndef JASMIN_LOTRI_H
#define JASMIN_LOTRI_H


#include "interface.h"


void LOTRI_Init(SDL_Renderer* renderer);
void LOTRI_Deinit();

void LOTRI_Load();
void LOTRI_Unload();

void LOTRI_Renew();
void LOTRI_Draw();


#endif //JASMIN_LOTRI_H