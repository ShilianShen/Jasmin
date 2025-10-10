#ifndef ENTITY_H
#define ENTITY_H


#include "intel.h"


enum EntityId {
    ENTITY_UNKNOWN,
    ENTITY_SOCRATES,
    ENTITY_HUMAN,
    ENTITY_DEATH,
    ENTITY_0,
    ENTITY_1,
    ENTITY_2,
    ENTITY_3,
    NUM_ENTITIES
};
typedef struct {
    const char* name;
    SDL_FPoint position;
    bool visible;
    SDL_Texture* tex;
    SDL_FPoint repulsion, gravitation, gravity;
} Entity;


extern Entity entitySet[NUM_ENTITIES];


bool INTEL_InitEntity();
void INTEL_ExitEntity();


#endif //ENTITY_H