#include "maze.h"

#include "block/block.h"


void MAZE_Init(SDL_Renderer* ren) {
    mazeRenderer = ren;
}
void MAZE_Deinit() {

}


void MAZE_Load() {
    GENE_Load();
    BLOCK_Load();
}
void MAZE_Unload() {
    GENE_Unload();
    BLOCK_Unload();
}


void MAZE_Renew() {
    BLOCK_Renew();
}
void MAZE_Draw() {
    BLOCK_Draw();
}