#include "buffer.h"


int modelBufferHead = 0;
const LOTRI_Model* modelBuffer[MAX_MODEL_BUFFER] = {0};


static bool LOTRI_DrawModelBuffer_Model(const LOTRI_Model* model) {
    if (model == NULL) return false;

    for (int i = 0; i < model->numFaces; i++) {
        if (model->worldFaces[i].xyz.v.z > 0) continue;
        const Vec3i face = model->modelFaces[i].ijk;

        SDL_RenderGeometryRaw(
            renderer, model->texture,
            (float*)&model->worldVertices[0].xyz, sizeof(LOTRI_Vertex),
            (SDL_FColor*)&model->worldVertices[0].rgba, sizeof(LOTRI_Vertex),
            (float*)&model->worldVertices[0].uv, sizeof(LOTRI_Vertex),
            model->numVertices, face.arr, 3, sizeof(int)
            );
    }
    return true;
}
bool LOTRI_DrawModelBuffer(const int N, const LOTRI_Model* modelArray[N]) {
    if (modelArray == NULL) return false;

    float depth[N];
    for (int i = 0; i < N; i++) {
        depth[i] = modelArray[i]->depth;
    }

    int indices[N];
    BASIC_SortIndices(N, depth, indices, false);

    bool result = true;
    for (int i = 0; i < N; i++) {
        result = result && LOTRI_DrawModelBuffer_Model(modelArray[indices[i]]);
    }
    return result;
}