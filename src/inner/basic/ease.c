#include "ease.h"


float ACTIV_None(const float x) {
    if (x >= 1) {return 1;}
    if (x <= 0) {return 0;}
    return x;
}
float ACTIV_Sin2(const float x) {
    if (x >= 1) {return 1;}
    if (x <= 0) {return 0;}
    return SDL_powf(SDL_sinf(3.14f / 2 * x), 2);
}
SDL_FRect ACTIV_GetFRect(const SDL_FRect rect1, const SDL_FRect rect2, const float t) {
    SDL_FRect rect;
    rect.x = (1 - t) * rect1.x + t * rect2.x;
    rect.y = (1 - t) * rect1.y + t * rect2.y;
    rect.w = (1 - t) * rect1.w + t * rect2.w;
    rect.h = (1 - t) * rect1.h + t * rect2.h;
    return rect;
}