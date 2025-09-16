#include "cube.h"


Model* cube = NULL;


bool LOTRI_InitCube() {
    cube = LOTRI_CreateModel("../res/model/root_room/root_room.obj", "../res/model/root_room/root_room.mtl");
    if (cube == NULL) {
        return false;
    }
    return true;
}