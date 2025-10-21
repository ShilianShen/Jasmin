#ifndef ENTITY_H
#define ENTITY_H


#include "intel.h"


typedef enum {
    ENTITY_NULL,
    ENTITY_SOMEONE,
    ENTITY_SOMETHING,
    ENTITY_SOCRATES,
    ENTITY_HUMAN,
    ENTITY_DEATH,
    ENTITY_FLY,
    NUM_ENTITIES
} EntityId;


extern const char* ENTITY_NAMES[NUM_ENTITIES];


#endif //ENTITY_H