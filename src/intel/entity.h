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
typedef struct {
    bool visible;
    SDL_FPoint repulsion, gravitation, gravity, position;
    SDL_FRect rect;
} Entity;

extern EntityId entityMoveId;
extern Trig trigMove;


extern Entity entitySet[NUM_ENTITIES];
extern const char* ENTITY_NAMES[NUM_ENTITIES];


void INTEL_ResetIntelNet();



bool INTEL_RenewEntity();



#endif //ENTITY_H