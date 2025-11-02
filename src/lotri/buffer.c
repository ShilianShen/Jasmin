#include "buffer.h"


#define LOTRI_MAX_BUFFER 64
static int worldBufferHead = 0;
static LOTRI_MW* worldBuffer[LOTRI_MAX_BUFFER] = {0};


// GET & SET ===========================================================================================================
bool LOTRI_ClearBuffer() {
    for (int i = 0; i < LOTRI_MAX_BUFFER; i++) {
        worldBuffer[i] = NULL;
    }
    worldBufferHead = 0;
    return true;
}
bool LOTRI_SendBuffer(LOTRI_MW* model) {
    if (model == NULL) return false;
    if (worldBufferHead >= LOTRI_MAX_BUFFER) return false;

    worldBuffer[worldBufferHead] = model;
    worldBufferHead++;
    return true;
}


// RENEW ===============================================================================================================
bool LOTRI_RenewBuffer() {
    return true;
}


// DRAW ================================================================================================================
bool LOTRI_DrawBuffer() {
    float depth[worldBufferHead];
    for (int i = 0; i < worldBufferHead; i++) {
        LOTRI_World* world = worldBuffer[i]->world;
        REQ_CONDITION(LOTRI_RenewWorld(world), return false);
        REQ_CONDITION(LOTRI_GetWorldDepth(world, &depth[i]), return false);
    }

    int indices[worldBufferHead];
    BASIC_SortIndices(worldBufferHead, depth, indices, false);

    for (int i = 0; i < worldBufferHead; i++) {
        const int I = indices[i];
        const LOTRI_World* world = worldBuffer[I]->world;
        REQ_CONDITION(LOTRI_DrawWorld(world), return false);
    }
    return true;
}