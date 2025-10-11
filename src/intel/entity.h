#ifndef ENTITY_H
#define ENTITY_H


#include "intel.h"


typedef enum {
    ENTITY_UNKNOWN,
    ENTITY_SOMEONE,
    ENTITY_SOMETHING,
    ENTITY_SOCRATES,
    ENTITY_HUMAN,
    ENTITY_DEATH,
    ENTITY_FLY,
    NUM_ENTITIES
} EntityId;
typedef struct {
    const char* name;
    bool visible;
    SDL_Texture* tex;
    SDL_FPoint repulsion, gravitation, gravity, position;
} Entity;


extern Entity entitySet[NUM_ENTITIES];


bool INTEL_InitEntity();
bool INTEL_RenewEntity();
bool INTEL_DrawEntity();
void INTEL_ExitEntity();


#endif //ENTITY_H