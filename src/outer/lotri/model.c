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
    Vec3f scale;
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
bool LOTRI_SetModelMat(Model* model, const Mat4f mat) {
    if (model == NULL) return false;

    model->mat = mat;
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
static bool LOTRI_CreateModel_RK(Model* model, const fastObjMesh* mesh, const char* file_mtl) {
    {
        model->numVertices = (int)mesh->position_count;
        model->modelVertices = calloc(model->numVertices, sizeof(Vec3f));
        model->worldVertices = calloc(model->numVertices, sizeof(Vec4f));
        model->finalVertices = calloc(model->numVertices, sizeof(SDL_Vertex));

        if (model->modelVertices == NULL ||
            model->worldVertices == NULL ||
            model->finalVertices == NULL) {
            printf("%s: Failed to allocate memory for LOTRI_CreateModel_RK\n", __func__);
            return false;
            }

        for (int i = 0; i < model->numVertices; i++) {
            model->modelVertices[i] = (Vec3f){
                mesh->positions[3*i],
                mesh->positions[3*i+1],
                mesh->positions[3*i+2]
            };
            model->finalVertices[i].color = (SDL_FColor){1, 1, 1, 1};
            model->finalVertices[i].tex_coord = (SDL_FPoint){mesh->texcoords[2*i], 1-mesh->texcoords[2*i+1]};
        }
    }
    {
        model->numFaces = (int)mesh->face_count;
        model->modelFaces = calloc(model->numFaces, sizeof(Vec3i));
        model->modelFaceNormals = calloc(model->numFaces, sizeof(Vec3f));
        model->worldFaceNormals = calloc(model->numFaces, sizeof(Vec4f));
        model->faceIndices = calloc(model->numFaces, sizeof(int));

        if (model->modelFaces == NULL ||
            model->modelFaceNormals == NULL ||
            model->worldFaceNormals == NULL ||
            model->faceIndices == NULL) {
            printf("%s: Failed to allocate memory for LOTRI_CreateModel_RK\n", __func__);
            return false;
            }

        for (int i = 0; i < model->numFaces; i++) {
            model->modelFaces[i] = (Vec3i){
                (int)mesh->indices[3*i].p,
                (int)mesh->indices[3*i+1].p,
                (int)mesh->indices[3*i+2].p,
            };
            model->modelFaceNormals[i] = (Vec3f){
                mesh->normals[3*i+3],
                mesh->normals[3*i+4],
                mesh->normals[3*i+5],
            };
        }
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
Model* LOTRI_CreateModel(const char* file_obj, const char *file_mtl) {

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
        printf("%s: Failed to create LOTRI model\n", __func__);
        LOTRI_DestroyModel(model);
        model = NULL;
    }
    fast_obj_destroy(mesh);
    LOTRI_SetModelScale(model, (Vec3f){1, 1, 1});

    return model;
}


// RENEW ===============================================================================================================
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
        LOTRI_GetMatS(model->scale),
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
        DEBUG_SendMessageR("%.2f, ", model->worldFaceNormals[i].v.x);
        DEBUG_SendMessageR("%.2f, ", model->worldFaceNormals[i].v.y);
        DEBUG_SendMessageR("%.2f\n", model->worldFaceNormals[i].v.z);

    }
    return true;
}