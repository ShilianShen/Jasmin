#include "lo_tri.h"


SDL_Vertex vert[3];
static SDL_Renderer* TEST_Renderer;
static SDL_Texture *texture = NULL;

static void printVertex(const SDL_Vertex vert) {
    printf("translation: (%.2f, %.2f)\n", vert.position.x, vert.position.y);
    printf("color: (%.2f, %.2f, %.2f, %.2f)\n", vert.color.r, vert.color.g, vert.color.b, vert.color.a);
    printf("tex_coord: (%.2f, %.2f)\n", vert.tex_coord.x, vert.tex_coord.y);
}
void TEST_Init(SDL_Renderer* renderer) {
    TEST_Renderer = renderer;
    if (texture == NULL) {
        texture = IMG_LoadTexture(TEST_Renderer, "/Users/shilianshen/Documents/Oolong Press/Lo-Tri/images/jasmin_background.jpg");
    }
    for (int i = 0; i < 3; i++) {
        printf("%d\n", i);
        printVertex(vert[i]);
    }
    // center
    vert[0].position.x = 400;
    vert[0].position.y = 150;
    vert[0].color.r = 1.0;
    vert[0].color.g = 0.0;
    vert[0].color.b = 1.0;
    vert[0].color.a = 1.0;
    vert[0].tex_coord.x = 0;
    vert[0].tex_coord.y = 0;

    // left
    vert[1].position.x = 200;
    vert[1].position.y = 450;
    vert[1].color.r = 1.0;
    vert[1].color.g = 1.0;
    vert[1].color.b = 1.0;
    vert[1].color.a = 1.0;
    vert[1].tex_coord.x = 0.5;
    vert[1].tex_coord.y = 0.5;

    // right
    vert[2].position.x = 600;
    vert[2].position.y = 450;
    vert[2].color.r = 1.0;
    vert[2].color.g = 1.0;
    vert[2].color.b = 1.0;
    vert[2].color.a = 1.0;
    vert[2].tex_coord.x = 0;
    vert[2].tex_coord.y = 0.5;
}
void TEST_Draw() {
    SDL_RenderGeometry(TEST_Renderer, texture, vert, 3, NULL, 0);
}