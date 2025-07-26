#ifndef JASMIN_BASIC_EASE_H
#define JASMIN_BASIC_EASE_H


/*
 * Ease提供一系列激发函数, 即主要在意从[0, 1]映射到[0, 1]的函数.
 * 因为t从0到1的过程, 可能是线性的, 所以为了让动画或者别的东西变化得更加平滑, 可以使用EASE函数.
 * t<0时, 返回0, t>1时返回1.
 */


#include "../setting/setting.h"


float EASE_None(const float x);
float EASE_Sin2(const float x);
SDL_FRect EASE_GetFRect(const SDL_FRect rect1, const SDL_FRect rect2, const float t);


#endif //JASMIN_BASIC_EASE_H
