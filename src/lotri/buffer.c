#include "buffer.h"


#define MAX_MODEL_BUFFER 64
static int modelBufferHead = 0;
static LOTRI_MW* modelBuffer[MAX_MODEL_BUFFER] = {0};


// GET & SET ===========================================================================================================
bool LOTRI_ClearBuffer() {
    for (int i = 0; i < MAX_MODEL_BUFFER; i++) {
        modelBuffer[i] = NULL;
    }
    modelBufferHead = 0;
    return true;
}
bool LOTRI_BufferModel(LOTRI_MW* model) {
    if (model == NULL) return false;
    if (modelBufferHead >= MAX_MODEL_BUFFER) return false;

    modelBuffer[modelBufferHead] = model;
    modelBufferHead++;
    return true;
}


// DRAW ================================================================================================================
bool LOTRI_DrawModelBuffer() {
    float depth[modelBufferHead];
    for (int i = 0; i < modelBufferHead; i++) {
        LOTRI_MW* model = modelBuffer[i];
        LOTRI_RenewModel(model);
        LOTRI_GetWorldDepth(model->world, &depth[i]);
    }

    int indices[modelBufferHead];
    BASIC_SortIndices(modelBufferHead, depth, indices, false);

    bool result = true;
    for (int i = 0; i < modelBufferHead; i++) {
        const int I = indices[i];
        const LOTRI_MW* model = modelBuffer[I];
        result = result && LOTRI_DrawModel(model);
    }
    return result;
}