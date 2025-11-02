#ifndef LOTRI_WORLD_H
#define LOTRI_WORLD_H


#include "_lotri.h"
#include "model.h"


bool LOTRI_GetWorldVertex(const LOTRI_World *world, int index, Vec3f* vec);
bool LOTRI_GetWorldDepth(const LOTRI_World *world, float* depth);
bool LOTRI_GetWorldPosition(const LOTRI_World *world, Vec3f* position);
bool LOTRI_SetWorldScale(LOTRI_World *world, Vec3f scale);
bool LOTRI_SetWorldSrc(LOTRI_World *world, SDL_FRect* src);
bool LOTRI_SetWorldPosition(LOTRI_World *world, Vec3f position);
bool LOTRI_SetWorldRotation(LOTRI_World *world, Vec3f rotation);


LOTRI_World* LOTRI_DeleteWorld(LOTRI_World* world);


#endif //LOTRI_WORLD_H