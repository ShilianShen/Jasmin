#include "model.h"


int modelBufferHead = 0;
const Model* modelBuffer[MAX_MODEL_BUFFER] = {0};


struct Model {
    Vec3f scale, position, rotation;

    int numVertices;
    LOTRI_Vertex *modelVertices, *worldVertices;

    int numFaces;
    LOTRI_Face *modelFaces, *worldFaces;

    ModelSide side;
    SDL_FRect* src;
    float depth;
    SDL_Texture* texture;
    // Mat4f mat;
};


// GET & SET ===========================================================================================================
bool LOTRI_GetModelPosition(const Model* model, Vec3f* position) {
    REQ_CONDITION(model != NULL, return false);
    REQ_CONDITION(position != NULL, return false);
    *position = model->position;
    return true;
}
bool LOTRI_GetModelWorldVertex(const Model* model, const int index, Vec3f* vec) {
    if (model == NULL) return false;
    if (index >= model->numVertices) return false;

    *vec = model->worldVertices[index].xyz;
    return true;
}
bool LOTRI_GetModelModelVertex(const Model* model, const int index, Vec3f* vec) {
    if (model == NULL) return false;
    if (index >= model->numVertices) return false;

    *vec = model->modelVertices[index].xyz;
    return true;
}


bool LOTRI_SetModelSrc(Model* model, SDL_FRect* src) {
    if (model == NULL) return false;

    model->src = src;
    return true;
}
bool LOTRI_SetModelScale(Model* model, const Vec3f scale) {
    REQ_CONDITION(model != NULL, return false);
    model->scale = scale;
    return true;
}
bool LOTRI_SetModelPosition(Model* model, const Vec3f position) {
    REQ_CONDITION(model != NULL, return false);
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
        const Vec3f normal = BASIC_GetNormal(a, b, c);
        const Vec3f sum = BASIC_GetSum(a, b, c);
        if (side == MODEL_SIDE_IN ^ BASIC_GetDot(normal, sum) < 0) {
            model->modelFaces[i].xyz = (Vec3f){-normal.v.x, -normal.v.y, -normal.v.z};
        }
        else {
            model->modelFaces[i].xyz = normal;
        }
    }
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
        BASIC_GetMatS(model->scale),
        BASIC_GetMatR(rotation),
        BASIC_GetMatT(model->position),
        camera.mat,
    };
    const Mat4f mat = BASIC_GetProd(len_of(matArr), matArr);

    for (int i = 0; i < model->numVertices; i++) {
        model->worldVertices[i].xyz = BASIC_GetV3M4(model->modelVertices[i].xyz, mat, true);
        model->worldVertices[i].uv = BASIC_GetV2Rect(model->modelVertices[i].uv, model->src);
    }
    for (int i = 0; i < model->numFaces; i++) {
        model->worldFaces[i].xyz = BASIC_GetV3M4(model->modelFaces[i].xyz, mat, false);
    }
}
static void LOTRI_RenewModel_Depth(Model* model) {
    float depths[model->numFaces];
    for (int i = 0; i < model->numFaces; i++) {
        const Vec3i index = model->modelFaces[i].ijk;
        float depth = 0;
        for (int j = 0; j < 3; j++) {
            const Vec3f vertex = model->worldVertices[index.arr[j]].xyz;
            depth = SDL_max(depth, vertex.v.z);
        }
        depths[i] = depth;
    }
    int indices[model->numFaces];
    BASIC_SortIndices(model->numFaces, depths, indices, false);
    for (int i = 0; i < model->numFaces; i++) {
        model->worldFaces[i].ijk = model->modelFaces[indices[i]].ijk;
    }



    if (model->side == MODEL_SIDE_CAMERA) {
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
    // LOTRI_RenewModel_WorldVertices(model);
    // LOTRI_RenewModel_WorldFaces(model);
    LOTRI_RenewModel_Depth(model);

    return true;
}


// DRAW ================================================================================================================
bool LOTRI_DrawModel(const Model* model) {
    if (model == NULL) return false;
    if (modelBufferHead >= MAX_MODEL_BUFFER) return false;

    modelBuffer[modelBufferHead] = model;
    modelBufferHead++;
    return true;
}
static bool LOTRI_DrawModelBuffer_Model(const Model* model) {
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
bool LOTRI_DrawModelBuffer(const int N, const Model* modelArray[N]) {
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