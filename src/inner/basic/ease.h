#ifndef JASMIN_BASIC_EASE_H
#define JASMIN_BASIC_EASE_H


#include "_basic.h"


typedef float (*AtvFunc)(float);
float BASIC_AtvLinear(float x);
float BASIC_AtvSin2(float x);
float BASIC_AtvRank1(float x);
float BASIC_AtvRank2(float x);
SDL_FRect BASIC_AtvGetFRect(SDL_FRect rect1, SDL_FRect rect2, float t);


#endif //JASMIN_BASIC_EASE_H