#ifndef ENTITY_H
#define ENTITY_H


#include "intel.h"


enum EntityId {
    ENTITY_UNKNOWN,
    ENTITY_SOCRATES,
    ENTITY_HUMAN,
    ENTITY_DEATH,
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
bool INTEL_RenewEntity(const IntelNet* intelNet);
bool INTEL_DrawEntity();


#endif //ENTITY_H