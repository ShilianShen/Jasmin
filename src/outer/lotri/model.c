#include "model.h"



typedef struct {int index; float key;} KeyIndex;
int compare_key_index(const void *a, const void *b) {
    const float diff = ((KeyIndex*)a)->key - ((KeyIndex*)b)->key;
    return (diff < 0) - (diff > 0);
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
    Vec3f scale;
    Vec3f position;
    Vec3f rotation;

    int numVertices;
    LOTRI_Vertex *modelVertices, *worldVertices;

    int numFaces;
    LOTRI_Face *modelFaces, *worldFaces;

    ModelSide side;
    SDL_FRect* src;
    float depth;
    SDL_Texture* texture;
    Mat4f mat;
};


// GET & SET ===========================================================================================================
bool LOTRI_SetModelScale(Model* model, const Vec3f scale) {
    if (model == NULL) {
        printf("LOTRI_SetModelPosition: NULL\n");
        return false;
    }

    model->scale = scale;
    return true;
}
bool LOTRI_SetModelPosition(Model* model, const Vec3f position) {
    if (model == NULL) {
        printf("LOTRI_SetModelPosition: NULL\n");
        return false;
    }

    model->position = position;
    return true;
}
bool LOTRI_SetModelRotation(Model* model, const Vec3f rotation) {
    if (model == NULL) {
        printf("LOTRI_SetModelRotation: NULL\n");
        return false;
    }

    model->rotation = rotation;
    return true;
}
bool LOTRI_SetModelNormals(const Model* model, const ModelSide side) {
    if (model == NULL) return false;
    if (side == MODEL_SIDE_NULL) return true;

    for (int i = 0; i < model->numFaces; i++) {
        const Vec3i index = model->modelFaces[i].ijk;
        const Vec3f a = model->modelVertices[index.v.i].xyz;
        const Vec3f b = model->modelVertices[index.v.j].xyz;
        const Vec3f c = model->modelVertices[index.v.k].xyz;
        const Vec3f normal = LOTRI_GetNormal(a, b, c);
        const Vec3f sum = LOTRI_GetSum(a, b, c);
        if (side == MODEL_SIDE_IN ^ LOTRI_GetDot(normal, sum) < 0) {
            model->modelFaces[i].xyz = (Vec3f){-normal.v.x, -normal.v.y, -normal.v.z};
        }
        else {
            model->modelFaces[i].xyz = normal;
        }
    }
    return true;
}
bool LOTRI_SetModelMat(Model* model, const Mat4f mat) {
    if (model == NULL) return false;

    model->mat = mat;
    return true;
}


bool LOTRI_GetModelCZ(const Model* model, float* cz) {
    if (model == NULL || cz == NULL) return false;

    *cz = model->rotation.v.z - camera.rotation.v.z;
    return true;
}
bool LOTRI_SetModelSrc(Model* model, SDL_FRect* src) {
    if (model == NULL) return false;

    model->src = src;
    return true;
}


// CREATE & DELETE =====================================================================================================
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
        if (model->texture != NULL) {
            SDL_DestroyTexture(model->texture);
            model->texture = NULL;
        }
        if (model->modelFaces != NULL) {
            free(model->modelFaces);
            model->modelFaces = NULL;
        }
        if (model->worldFaces != NULL) {
            free(model->worldFaces);
            model->worldFaces = NULL;
        }
        free(model);
    }
}
static bool LOTRI_CreateModel_RK(Model* model, const fastObjMesh* mesh, const char* file_mtl) {
    memset(model, 0, sizeof(Model));
    model->numVertices = (int)mesh->position_count;
    model->modelVertices = calloc(model->numVertices, sizeof(LOTRI_Vertex));
    model->worldVertices = calloc(model->numVertices, sizeof(LOTRI_Vertex));
    if (model->modelVertices == NULL || model->worldVertices == NULL) {
        printf("%s: Failed to allocate memory for LOTRI_CreateModel_RK\n", __func__);
        return false;
        }
    for (int i = 0; i < model->numVertices; i++) {
        model->modelVertices[i].xyz = (Vec3f){mesh->positions[3*i], mesh->positions[3*i+1], mesh->positions[3*i+2]};
        model->worldVertices[i].rgba = model->modelVertices[i].rgba = (Vec4f){1, 1, 1, 1};
        model->worldVertices[i].uv = model->modelVertices[i].uv = (Vec2f){mesh->texcoords[2*i], 1-mesh->texcoords[2*i+1]};
    }

    model->numFaces = (int)mesh->face_count;
    model->modelFaces = calloc(model->numFaces, sizeof(LOTRI_Face));
    model->worldFaces = calloc(model->numFaces, sizeof(LOTRI_Face));
    if (model->worldFaces == NULL || model->modelFaces == NULL) {
        printf("%s: Failed to allocate memory for LOTRI_CreateModel_RK\n", __func__);
        return false;
        }
    for (int i = 0; i < model->numFaces; i++) {
        model->modelFaces[i].ijk = (Vec3i){(int)mesh->indices[3*i].p, (int)mesh->indices[3*i+1].p, (int)mesh->indices[3*i+2].p,};
        model->modelFaces[i].xyz = (Vec3f){mesh->normals[3*i+3], mesh->normals[3*i+4], mesh->normals[3*i+5],};
    }

    MTLMaterial materials[1];
    const int num_materials = parse_mtl_file(file_mtl, materials, 1);
    model->texture = IMG_LoadTexture(renderer, materials[0].map_Kd);
    if (model->texture == NULL) {
        printf("%s: Failed to load texture\n", __func__);
        return false;
    }
    SDL_SetTextureScaleMode(model->texture, SDL_SCALEMODE_NEAREST);

    return true;
}
Model* LOTRI_CreateModel(const char* file_obj, const char *file_mtl, const ModelSide side) {

    Model* model = malloc(sizeof(Model));
    if (model == NULL) {
        printf("%s: Failed to \n", __func__);
        return false;
    }

    fastObjMesh* mesh = fast_obj_read(file_obj);
    if (mesh == NULL) {
        printf("%s: Failed to read OBJ file\n", __func__);
        return false;
    }

    if (LOTRI_CreateModel_RK(model, mesh, file_mtl) == false) {
        printf("%s: Failed to create LOTRI modelVertices\n", __func__);
        LOTRI_DestroyModel(model);
        model = NULL;
    }
    else {
        model->scale = (Vec3f){1, 1, 1};
        model->side = side;
        LOTRI_SetModelNormals(model, model->side);
    }
    fast_obj_destroy(mesh);


    return model;
}


// RENEW ===============================================================================================================
static void LOTRI_RenewModel_Mat(Model* model) {
    Vec3f rotation = model->rotation;
    if (model->side == MODEL_SIDE_CAMERA) {
        rotation = (Vec3f){0, -camera.rotation.v.y, camera.rotation.v.z + (float)M_PI};
    }
    const Mat4f matArr[] = {
        LOTRI_GetMatS(model->scale),
        LOTRI_GetMatR(rotation),
        LOTRI_GetMatT(model->position),
        camera.mat,
    };
    LOTRI_SetModelMat(model, LOTRI_GetProd(len_of(matArr), matArr));
}
static void LOTRI_RenewModel_WorldVertices(const Model* model) {
    // xyz
    for (int i = 0; i < model->numVertices; i++) {
        for (int j = 0; j < 3; j++) {
            float* ij = &model->worldVertices[i].xyz.arr[j];
            *ij = 0;
            for (int k = 0; k < 3; k++) {
                *ij += model->modelVertices[i].xyz.arr[k] * model->mat.m[k][j];
            }
            *ij += model->mat.m[3][j];
        }
    }
    // uv
    if (model->src == NULL) {
        for (int i = 0; i < model->numVertices; i++) {
            model->worldVertices[i].uv = model->modelVertices[i].uv;
        }
    }
    else {
        for (int i = 0; i < model->numVertices; i++) {
            model->worldVertices[i].uv = (Vec2f){
                model->src->x + model->src->w * model->modelVertices[i].uv.v.x,
                model->src->y + model->src->h * model->modelVertices[i].uv.v.y,
            };
        }
    }
}
static void LOTRI_RenewModel_WorldFaces(const Model* model) {
    for (int i = 0; i < model->numFaces; i++) {
        for (int j = 0; j < 3; j++) {
            float* ij = &model->worldFaces[i].xyz.arr[j];
            *ij = 0;
            for (int k = 0; k < 3; k++) {
                *ij += model->modelFaces[i].xyz.arr[k] * model->mat.m[k][j];
            }
        }
    }
    float depths[model->numFaces];
    int indices[model->numFaces];
    for (int i = 0; i < model->numFaces; i++) {
        const Vec3i index = model->modelFaces[i].ijk;
        float depth = 0;
        for (int j = 0; j < 3; j++) {
            const Vec3f vertex = model->worldVertices[index.arr[j]].xyz;
            depth = SDL_max(depth, vertex.v.z);
        }
        depths[i] = depth;
    }
    sort_indices_by_keys(model->numFaces, depths, indices);
    for (int i = 0; i < model->numFaces; i++) {
        model->worldFaces[i].ijk = model->modelFaces[indices[i]].ijk;
    }
}
static void LOTRI_RenewModel_Depth(Model* model) {
    if (model->side == MODEL_SIDE_NULL) {
        model->depth = model->worldVertices[0].xyz.v.z;
        return;
    }
    float depth = 0;
    for (int i = 0; i < model->numVertices; i++) {
        const Vec3f vertex = model->worldVertices[i].xyz;
        if (model->side == MODEL_SIDE_IN) {
            depth = SDL_max(depth, vertex.v.z);
        }
        else {
            depth = SDL_min(depth, vertex.v.z);
        }
    }
    model->depth = depth;
}
bool LOTRI_RenewModel(Model* model) {
    if (model == NULL) return false;

    LOTRI_RenewModel_Mat(model);
    LOTRI_RenewModel_WorldVertices(model);
    LOTRI_RenewModel_WorldFaces(model);
    LOTRI_RenewModel_Depth(model);

    return true;
}
bool LOTRI_RenewModelArray(const int N, Model* modelArray[N]) {
    if (modelArray == NULL) return false;
    bool result = true;
    for (int i = 0; i < N; i++) {
        result = result && LOTRI_RenewModel(modelArray[i]);
    }
    return result;
}


// DRAW ================================================================================================================
bool LOTRI_DrawModel(const Model* model) {
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
bool LOTRI_DrawModelArray(const int N, const Model* modelArray[N]) {
    if (modelArray == NULL) return false;

    float depth[N];
    for (int i = 0; i < N; i++) {
        depth[i] = modelArray[i]->depth;
    }

    int indices[N];
    sort_indices_by_keys(N, depth, indices);

    bool result = true;
    for (int i = 0; i < N; i++) {
        result = result && LOTRI_DrawModel(modelArray[indices[i]]);
    }
    return result;
}