#ifndef VILLA_ROOM_H
#define VILLA_ROOM_H


#include "villa.h"


typedef struct Room Room;
typedef struct Coord Coord;
struct Coord {
    Room *room;
    int x, y;
};


bool VILLA_GetRoomCellEmpty(Coord coord);
bool VILLA_GetRoomCellPosition(Coord coord, Vec3f* position);

void *VILLA_CreateRoom(const cJSON *room_json);
void VILLA_DestroyRoom_V(void* room_void);

void VILLA_DeleteRoom(void *room_void);
bool VILLA_RenewRoom(void *room_void);
bool VILLA_DrawRoom(const void *room_void);


#endif //VILLA_ROOM_H