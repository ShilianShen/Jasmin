#include "lo_tri.h"


SDL_Vertex vert[3];


void TEST_Init() {
    // center
    vert[0].position.x = 400;
    vert[0].position.y = 150;
    vert[0].color.r = 1.0;
    vert[0].color.g = 0.0;
    vert[0].color.b = 0.0;
    vert[0].color.a = 1.0;

    // left
    vert[1].position.x = 200;
    vert[1].position.y = 450;
    vert[1].color.r = 0.0;
    vert[1].color.g = 0.0;
    vert[1].color.b = 1.0;
    vert[1].color.a = 1.0;

    // right
    vert[2].position.x = 600;
    vert[2].position.y = 450;
    vert[2].color.r = 0.0;
    vert[2].color.g = 1.0;
    vert[2].color.b = 0.0;
    vert[2].color.a = 1.0;
}