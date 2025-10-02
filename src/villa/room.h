#ifndef VILLA_ROOM_H
#define VILLA_ROOM_H


#include "villa.h"


typedef enum {
    VILLA_DIRECT_PX,
    VILLA_DIRECT_PY,
    VILLA_DIRECT_NX,
    VILLA_DIRECT_NY,
    VILLA_NUM_DIRECTS,
} VILLA_DIRECT;
const char* VILLA_GetStrDirect(int direct);



typedef enum {
    VILLA_ACT_NONE,
    VILLA_ACT_WALK_1,
    VILLA_ACT_WALK_2,
    VILLA_ACT_MOVE_aa,
    VILLA_NUM_ACTS,
} VILLA_Action;
typedef struct Room Room;
typedef struct Coord Coord;
struct Coord {
    Room *room;
    int x, y;
    int direct;
};


bool VILLA_GetRoomCellEmpty(Coord coord);
bool VILLA_GetRoomCellPosition(Coord coord, Vec3f* position);

void *VILLA_CreateRoom(const cJSON *room_json);

void *VILLA_DestroyRoom_V(void *room_void);

void VILLA_DeleteRoom(void *room_void);
bool VILLA_RenewRoom(void *room_void);
bool VILLA_DrawRoom(const void *room_void);


#endif //VILLA_ROOM_H