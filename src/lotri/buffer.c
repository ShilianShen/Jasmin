#include "buffer.h"


int modelBufferHead = 0;
const LOTRI_Model* modelBuffer[MAX_MODEL_BUFFER] = {0};


// GET & SET ===========================================================================================================
bool LOTRI_BufferModel(const LOTRI_Model* model) {
    if (model == NULL) return false;
    LOTRI_RenewModel(model);
    if (modelBufferHead >= MAX_MODEL_BUFFER) return false;

    modelBuffer[modelBufferHead] = model;
    modelBufferHead++;
    return true;
}


// RENEW ===============================================================================================================
bool LOTRI_RenewModelBuffer() {
    return true;
}


// DRAW ================================================================================================================
bool LOTRI_DrawModelBuffer() {
    float depth[modelBufferHead];
    for (int i = 0; i < modelBufferHead; i++) {
        depth[i] = modelBuffer[i]->depth;
    }

    int indices[modelBufferHead];
    BASIC_SortIndices(modelBufferHead, depth, indices, false);

    bool result = true;
    for (int i = 0; i < modelBufferHead; i++) {
        result = result && LOTRI_DrawModel(modelBuffer[indices[i]]);
    }
    return result;
}