#ifndef JASMIN_BASIC_EASE_H
#define JASMIN_BASIC_EASE_H


/*
 * Ease提供一系列激发函数, 即主要在意从[0, 1]映射到[0, 1]的函数.
 * 因为t从0到1的过程, 可能是线性的, 所以为了让动画或者别的东西变化得更加平滑, 可以使用EASE函数.
 * t<0时, 返回0, t>1时返回1.
 */

#include "setting.h"


float ACTIV_None(float x);
float ACTIV_Sin2(float x);
SDL_FRect ACTIV_GetFRect(SDL_FRect rect1, SDL_FRect rect2, float t);


#endif //JASMIN_BASIC_EASE_H
