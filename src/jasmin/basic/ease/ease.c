#include "ease.h"


float EASE_None(const float x) {
    if (x >= 1) {return 1;}
    if (x <= 0) {return 0;}
    return x;
}
float EASE_Sin2(const float x) {
    if (x >= 1) {return 1;}
    if (x <= 0) {return 0;}
    return SDL_powf(SDL_sinf(3.14f / 2 * x), 2);
}