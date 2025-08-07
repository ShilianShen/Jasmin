#include "maze_inner.h"

#include "room/room.h"


void MAZE_INNER_Init(SDL_Renderer* ren) {
    mazeRenderer = ren;
}
void MAZE_INNER_Deinit() {

}


void MAZE_INNER_Load() {
    GENE_Load();
    ROOM_Load();
    ITEM_Load();
    MONSTER_Load();
}
void MAZE_INNER_Unload() {
    MONSTER_Unload();
    ITEM_Unload();
    GENE_Unload();
    ROOM_Unload();
}


void MAZE_INNER_Renew() {
    ROOM_Renew();
    MONSTER_Renew();
}
void MAZE_INNER_Draw() {
}