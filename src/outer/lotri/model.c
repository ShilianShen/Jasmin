#include "model.h"



typedef struct {int index; float key;} KeyIndex;
int compare_key_index(const void *a, const void *b) {
    const float diff = ((KeyIndex*)a)->key - ((KeyIndex*)b)->key;
    return ((diff < 0) - (diff > 0));
}
void sort_indices_by_keys(const int N, const float keys[N], int indices[N]) {
    KeyIndex temp[N];

    // 初始化索引和对应的 key 值
    for (int i = 0; i < N; ++i) {
        temp[i].index = i;
        temp[i].key = keys[i];
    }

    // 排序
    qsort(temp, N, sizeof(KeyIndex), compare_key_index);

    // 提取排序后的索引
    for (int i = 0; i < N; ++i) {
        indices[i] = temp[i].index;
    }
}




bool LOTRI_SetModelNormals(const Model* model, const bool in) {
    if (model == NULL) return false;

    for (int i = 0; i < model->numFaces; i++) {
        const Vec3i index = model->modelFaces[i];
        const Vec3f a = model->modelVertices[index.v.a];
        const Vec3f b = model->modelVertices[index.v.b];
        const Vec3f c = model->modelVertices[index.v.c];
        const Vec3f normal = LOTRI_GetNormal(a, b, c);
        const Vec3f sum = LOTRI_GetSum(a, b, c);
        if (in ^ LOTRI_GetDot(normal, sum) < 0) {
            model->modelFaceNormals[i] = (Vec3f){-normal.v.x, -normal.v.y, -normal.v.z};
        }
        else {
            model->modelFaceNormals[i] = normal;
        }
    }
    return true;
}


static void LOTRI_RenewModel_Depth(Model* model) {
    float depth = 0;
    for (int i = 0; i < model->numVertices; i++) {
        depth += model->worldVertices[i].v.z;
    }
    depth /= (float)model->numVertices;
    model->depth = depth;
}
static void LOTRI_RenewModel_FaceIndices(const Model* model) {
    float depths[model->numFaces];
    for (int i = 0; i < model->numFaces; i++) {
        const Vec3i index = model->modelFaces[i];
        float depth = 0;
        for (int j = 0; j < 3; j++) {
            const Vec4f vertex = model->worldVertices[index.arr[j]];
            depth += vertex.v.z;
        }
        depths[i] = depth;
    }
    sort_indices_by_keys(model->numFaces, depths, model->faceIndices);
}
bool LOTRI_RenewModel(Model* model) {
    if (model == NULL) return false;

    LOTRI_LoadV3M4(model->numVertices, model->modelVertices, model->mat, model->worldVertices, true);
    LOTRI_LoadV3M4(model->numFaces, model->modelFaceNormals, model->mat, model->worldFaceNormals, false);

    for (int i = 0; i < model->numVertices; i++) {
        model->finalVertices[i].position.x = model->worldVertices[i].v.x;
        model->finalVertices[i].position.y = model->worldVertices[i].v.y;
    }

    LOTRI_RenewModel_Depth(model);
    LOTRI_RenewModel_FaceIndices(model);

    return true;
}
bool LOTRI_DrawModel(const Model* model) {
    if (model == NULL) return false;

    for (int i = 0; i < model->numFaces; i++) {
        if (model->worldFaceNormals[i].v.z <= 0) continue;

        SDL_RenderGeometry(
           renderer, model->texture,
           model->finalVertices, model->numVertices,
           model->modelFaces[i].arr, 3
           );
    }
    return true;
}