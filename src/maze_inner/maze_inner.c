#include "maze_inner.h"

#include "block/block.h"


void MAZE_INNER_Init(SDL_Renderer* ren) {
    mazeRenderer = ren;
}
void MAZE_INNER_Deinit() {

}


void MAZE_INNER_Load() {
    GENE_Load();
    BLOCK_Load();
    MONSTER_Load();
}
void MAZE_INNER_Unload() {
    MONSTER_Unload();
    GENE_Unload();
    BLOCK_Unload();
}


void MAZE_INNER_Renew() {
    BLOCK_Renew();
    MONSTER_Renew();
}
void MAZE_INNER_Draw() {
    BLOCK_Draw_BDI();
    MONSTER_Draw();
}