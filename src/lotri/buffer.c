#include "buffer.h"


int modelBufferHead = 0;
LOTRI_Model* modelBuffer[MAX_MODEL_BUFFER] = {0};


// GET & SET ===========================================================================================================
bool LOTRI_BufferModel(LOTRI_Model* model) {
    if (model == NULL) return false;
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
        LOTRI_Model* model = modelBuffer[i];
        LOTRI_RenewModel(model);
        LOTRI_GetModelDepth(model, &depth[i]);
    }

    int indices[modelBufferHead];
    BASIC_SortIndices(modelBufferHead, depth, indices, false);

    bool result = true;
    for (int i = 0; i < modelBufferHead; i++) {
        const int I = indices[i];
        const LOTRI_Model* model = modelBuffer[I];
        result = result && LOTRI_DrawModel(model);
    }
    return result;
}