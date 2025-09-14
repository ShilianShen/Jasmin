#ifndef VILLA_ROOM_H
#define VILLA_ROOM_H


#include "villa.h"


typedef struct Room Room;


Room* VILLA_CreateRoom();
Room* VILLA_DeleteRoom();
bool VILLA_RenewRoom(Room* room);
bool VILLA_DrawRoom(Room* room);


#endif //VILLA_ROOM_H