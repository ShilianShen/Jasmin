#ifndef LOTRI_WORLD_H
#define LOTRI_WORLD_H


#include "_lotri.h"
#include "model.h"


typedef struct LOTRI_World LOTRI_World;
struct LOTRI_World {
    const LOTRI_Model* model;
    Vec3f scale, position, rotation;
    LOTRI_Vertex *vertices;
    LOTRI_Face *faces;
    SDL_FRect* src;
    float depth;
};


bool LOTRI_GetWorldVertex(const LOTRI_World *world, int index, Vec3f* vec);
bool LOTRI_GetWorldDepth(const LOTRI_World *world, float* depth);
bool LOTRI_GetWorldPosition(const LOTRI_World *world, Vec3f* position);
bool LOTRI_SetWorldScale(LOTRI_World *world, Vec3f scale);
bool LOTRI_SetWorldSrc(LOTRI_World *world, SDL_FRect* src);
bool LOTRI_SetWorldPosition(LOTRI_World *world, Vec3f position);
bool LOTRI_SetWorldRotation(LOTRI_World *world, Vec3f rotation);


LOTRI_World* LOTRI_CreateWorld(const LOTRI_Model* model);
LOTRI_World* LOTRI_DeleteWorld(LOTRI_World* world);


#endif //LOTRI_WORLD_H