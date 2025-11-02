#include "world.h"


// GET & SET ===========================================================================================================
bool LOTRI_GetWorldPosition(const LOTRI_World *world, Vec3f* position) {
    REQ_CONDITION(world != NULL, return false);
    REQ_CONDITION(position != NULL, return false);
    *position = world->position;
    return true;
}
bool LOTRI_GetWorldVertex(const LOTRI_World *world, const int index, Vec3f* vec) {
    REQ_CONDITION(world != NULL, return false);
    REQ_CONDITION(vec != NULL, return false);
    if (index >= world->model->numVertices) return false;
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
    REQ_CONDITION(world != NULL, return false);
    world->src = src;
    return true;
}


// CREATE & DELETE =====================================================================================================
static bool LOTRI_CreateWorld_RK(LOTRI_World* world, const LOTRI_Model* model) {
    world->model = model;
    world->vertices = calloc(model->numVertices, sizeof(LOTRI_Vertex));
    REQ_CONDITION(world->vertices != NULL, return false);
    for (int i = 0; i < model->numVertices; i++) {
        world->vertices[i].rgba = model->vertices[i].rgba;
        world->vertices[i].uv = model->vertices[i].uv;
    }
    world->faces = calloc(model->numFaces, sizeof(LOTRI_Face));
    REQ_CONDITION(world->faces != NULL, return false);
    world->scale = (Vec3f){1, 1, 1};
    return true;
}
LOTRI_World* LOTRI_CreateWorld(const LOTRI_Model* model) {
    LOTRI_World* world = calloc(1, sizeof(LOTRI_World));
    REQ_CONDITION(world != NULL, return NULL);

    const bool RK = LOTRI_CreateWorld_RK(world, model);
    REQ_CONDITION(RK, {world = LOTRI_DeleteWorld(world); return NULL;});

    return world;
}
LOTRI_World* LOTRI_DeleteWorld(LOTRI_World* world) {
    if (world == NULL) return world;

    if (world->vertices != NULL) {
        free(world->vertices);
        world->vertices = NULL;
    }
    if (world->faces != NULL) {
        free(world->faces);
        world->faces = NULL;
    }

    free(world);
    world = NULL;
    return world;
}


// RENEW ===============================================================================================================
static void LOTRI_RenewWorld_Depth(LOTRI_World* world) {
    float depth = 0;
    switch (world->model->side) {
        case MODEL_SIDE_IN: {
            for (int i = 0; i < world->model->numVertices; i++) {
                depth = SDL_max(depth, world->vertices[i].xyz.v.z);
            }
            break;
        }
        case MODEL_SIDE_OUT: {
            for (int i = 0; i < world->model->numVertices; i++) {
                depth = SDL_min(depth, world->vertices[i].xyz.v.z);
            }
            break;
        }
        default: {
            for (int i = 0; i < world->model->numVertices; i++) {
                depth += world->vertices[i].xyz.v.z;
            }
            depth /= (float)world->model->numVertices;
            break;
        }
    }
    world->depth = depth;
}
bool LOTRI_RenewWorld(LOTRI_World* world) {
    REQ_CONDITION(world != NULL, return false);

    if (world->model->side == MODEL_SIDE_CAMERA) {
        world->rotation = (Vec3f){0, -camera.rotation.v.y, camera.rotation.v.z + (float)M_PI};
    }

    const Mat4f matArr[] = {
        BASIC_GetMatS(world->scale),
        BASIC_GetMatR(world->rotation),
        BASIC_GetMatT(world->position),
        camera.mat,
    };
    const Mat4f mat = BASIC_GetProd(len_of(matArr), matArr);

    for (int i = 0; i < world->model->numVertices; i++) {
        world->vertices[i].xyz = BASIC_GetV3M4(world->model->vertices[i].xyz, mat, true);
        world->vertices[i].uv = BASIC_GetV2Rect(world->model->vertices[i].uv, world->src);
    }

    for (int i = 0; i < world->model->numFaces; i++) {
        world->faces[i].ijk = world->model->faces[i].ijk;
        world->faces[i].xyz = BASIC_GetV3M4(world->model->faces[i].xyz, mat, false);
    }
    LOTRI_RenewWorld_Depth(world);
    return true;
}


// DRAW ================================================================================================================
bool LOTRI_DrawWorld(const LOTRI_World* world) {
    REQ_CONDITION(world != NULL, return false);

    for (int i = 0; i < world->model->numFaces; i++) {
        if (world->faces[i].xyz.v.z > 0) continue;
        const Vec3i face = world->model->faces[i].ijk;

        SDL_RenderGeometryRaw(
            renderer, world->model->texture,
            (float*)&world->vertices[0].xyz, sizeof(LOTRI_Vertex),
            (SDL_FColor*)&world->vertices[0].rgba, sizeof(LOTRI_Vertex),
            (float*)&world->vertices[0].uv, sizeof(LOTRI_Vertex),
            world->model->numVertices, face.arr, 3, sizeof(int)
            );
    }
    return true;
}