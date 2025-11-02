#include "world.h"

// GET & SET ===========================================================================================================
bool LOTRI_GetWorldPosition(const LOTRI_World *world, Vec3f* position) {
    REQ_CONDITION(world != NULL, return false);
    REQ_CONDITION(position != NULL, return false);
    *position = world->position;
    return true;
}
bool LOTRI_GetWorldVertex(const LOTRI_World *world, const int index, Vec3f* vec) {
    if (world == NULL) return false;
    if (index >= world->numVertices) return false;
    *vec = world->vertices[index].xyz;
    return true;
}
bool LOTRI_GetWorldDepth(const LOTRI_World *world, float* depth) {
    REQ_CONDITION(world != NULL, return false);
    REQ_CONDITION(depth != NULL, return false);
    *depth = world->depth;
    return true;
}
bool LOTRI_SetWorldScale(LOTRI_World *world, const Vec3f scale) {
    REQ_CONDITION(world != NULL, return false);
    world->scale = scale;
    return true;
}
bool LOTRI_SetWorldPosition(LOTRI_World *world, const Vec3f position) {
    REQ_CONDITION(world != NULL, return false);
    world->position = position;
    return true;
}
bool LOTRI_SetWorldRotation(LOTRI_World *world, const Vec3f rotation) {
    REQ_CONDITION(world != NULL, return false);
    world->rotation = rotation;
    return true;
}
bool LOTRI_SetWorldSrc(LOTRI_World *world, SDL_FRect* src) {
    if (world == NULL) return false;
    world->src = src;
    return true;
}


// CREATE & DELETE =====================================================================================================
LOTRI_World* LOTRI_DeleteWorld(LOTRI_World* world) {
    if (world == NULL) {
        return world;
    }
    if (world->vertices != NULL) {
        free(world->vertices);
        world->vertices = NULL;
    }
    if (world->faces != NULL) {
        free(world->faces);
        world->faces = NULL;
    }
    return world;
}


// RENEW ===============================================================================================================
// DRAW ================================================================================================================
