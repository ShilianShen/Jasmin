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



struct Model {
    Vec3f position;
    Vec3f rotation;

    int numVertices;
    Vec3f* modelVertices;
    Vec4f* worldVertices;
    SDL_Vertex* finalVertices;

    int numFaces;
    Vec3i* modelFaces;
    Vec3f* modelFaceNormals;
    Vec4f* worldFaceNormals;

    int* faceIndices;

    float depth;
    SDL_Texture* texture;
    Mat4f mat;
};


// GET & SET ===========================================================================================================
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
bool LOTRI_SetModelVertices(const Model* model, const int N, const Vec3f xyz[N]) {
    if (model == NULL) return false;

    for (int i = 0; i < model->numVertices; ++i) {
        model->modelVertices[i] = xyz[i];
    }
    return true;
}
bool LOTRI_SetModelFaces(const Model* model, const int N, const Vec3i abc[N]) {
    if (model == NULL) return false;

    for (int i = 0; i < model->numFaces; ++i) {
        model->modelFaces[i] = abc[i];
    }
    return true;
}
bool LOTRI_SetModelTexture(Model* model, const char* filename) {
    if (model == NULL) return false;

    model->texture = IMG_LoadTexture(renderer, filename);
    if (model->texture == NULL) return false;
    return true;
}
bool LOTRI_SetModelUV(const Model* model, const int N, const Vec2f uv[N]) {
    if (model == NULL) return false;

    for (int i = 0; i < model->numVertices; ++i) {
        model->finalVertices[i].tex_coord.x = uv[i].v.x;
        model->finalVertices[i].tex_coord.y = uv[i].v.y;
    }
    return true;
}
bool LOTRI_SetModelMat(Model* model, const Mat4f mat) {
    if (model == NULL) return false;

    model->mat = mat;
    return true;
}


// CREATE & DELETE =====================================================================================================
static bool LOTRI_CreateModel_RK(Model* model, const int numVertices, const int numFaces) {
    memset(model, 0, sizeof(Model));

    model->numVertices = numVertices;
    model->modelVertices = calloc(numVertices, sizeof(Vec3f));
    model->worldVertices = calloc(numVertices, sizeof(Vec4f));
    model->finalVertices = calloc(numVertices, sizeof(SDL_Vertex));
    if (model->modelVertices == NULL ||
        model->worldVertices == NULL ||
        model->finalVertices == NULL) {
        printf("%s: Failed to allocate memory for LOTRI_CreateModel_RK\n", __func__);
        return false;
    }

    model->numFaces = numFaces;
    model->modelFaces = calloc(numFaces, sizeof(Vec3i));
    model->modelFaceNormals = calloc(numFaces, sizeof(Vec3f));
    model->worldFaceNormals = calloc(numFaces, sizeof(Vec4f));
    if (model->modelFaces == NULL ||
        model->modelFaceNormals == NULL ||
        model->worldFaceNormals == NULL) {
        printf("%s: Failed to allocate memory for LOTRI_CreateModel_RK\n", __func__);
        return false;
    }

    model->faceIndices = calloc(numFaces, sizeof(int));
    if (model->faceIndices == NULL) {
        printf("%s: Failed to allocate memory for LOTRI_CreateModel_RK\n", __func__);
        return false;
    }

    for (int i = 0; i < model->numVertices; i++) {
        model->finalVertices[i].color = (SDL_FColor){1, 1, 1, 1};
    }

    return true;
}
Model* LOTRI_CreateModel(const int numVertices, const int numFaces) {
    Model* model = malloc(sizeof(Model));
    if (model == NULL) {
        printf("%s: Failed to allocate memory for LOTRI model\n", __func__);
        return model;
    }
    if (LOTRI_CreateModel_RK(model, numVertices, numFaces) == false) {
        LOTRI_DestroyModel(model);
        model = NULL;
    }
    return model;
}
void LOTRI_DestroyModel(Model* model) {
    if (model != NULL) {
        if (model->modelVertices != NULL) {
            free(model->modelVertices);
            model->modelVertices = NULL;
        }
        if (model->worldVertices != NULL) {
            free(model->worldVertices);
            model->worldVertices = NULL;
        }
        if (model->finalVertices != NULL) {
            free(model->finalVertices);
            model->finalVertices = NULL;
        }
        if (model->modelFaces != NULL) {
            free(model->modelFaces);
            model->modelFaces = NULL;
        }
        if (model->modelFaceNormals != NULL) {
            free(model->modelFaceNormals);
            model->modelFaceNormals = NULL;
        }
        if (model->worldFaceNormals != NULL) {
            free(model->worldFaceNormals);
            model->worldFaceNormals = NULL;
        }
        if (model->faceIndices != NULL) {
            free(model->faceIndices);
            model->faceIndices = NULL;
        }
        if (model->texture != NULL) {
            SDL_DestroyTexture(model->texture);
            model->texture = NULL;
        }
        free(model);
    }
}


// RENEW ===============================================================================================================
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
            depth = SDL_max(depth, vertex.v.z);
        }
        depths[i] = depth;
    }
    sort_indices_by_keys(model->numFaces, depths, model->faceIndices);
}
bool LOTRI_RenewModel(Model* model) {
    if (model == NULL) return false;

    const Mat4f matArr[] = {
        LOTRI_GetMatR(model->rotation),
        LOTRI_GetMatT(model->position),
        LOTRI_GetInvT(camera.position),
        LOTRI_GetInvR(camera.rotation),
        matProj,
    };

    LOTRI_SetModelMat(model, LOTRI_GetProd(sizeof(matArr) / sizeof(Mat4f), matArr));
    LOTRI_LoadV3M4(model->numVertices, model->modelVertices, model->mat, model->worldVertices, true);
    LOTRI_LoadV3M4(model->numFaces, model->modelFaceNormals, model->mat, model->worldFaceNormals, false);

    for (int i = 0; i < model->numVertices; i++) {
        model->finalVertices[i].position.x = (float)windowWidth / 2 + model->worldVertices[i].v.x;
        model->finalVertices[i].position.y = (float)windowHeight / 2 + model->worldVertices[i].v.y;
    }

    LOTRI_RenewModel_Depth(model);
    LOTRI_RenewModel_FaceIndices(model);

    return true;
}


// DRAW ================================================================================================================
bool LOTRI_DrawModel(const Model* model) {
    if (model == NULL) return false;

    for (int i = 0; i < model->numFaces; i++) {
        if (model->worldFaceNormals[i].v.z > 0) continue;

        const Vec3i face = model->modelFaces[i];

        SDL_RenderGeometry(
           renderer, model->texture,
           model->finalVertices, model->numVertices,
           face.arr, 3
           );
    }
    return true;
}