#ifndef VILLA_ROOM_H
#define VILLA_ROOM_H


#include "villa.h"


typedef struct Room Room;


bool VILLA_GetRoomCellEmpty(const Room* room, int x, int y);

void *VILLA_CreateRoom(const cJSON *room_json);
void VILLA_DestroyRoom_V(void* room_void);
Room* VILLA_DeleteRoom(Room *room);
bool VILLA_RenewRoom(void *room_void);
bool VILLA_DrawRoom(const void *room_void);


#endif //VILLA_ROOM_H