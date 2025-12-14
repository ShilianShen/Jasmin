#include "model.h"


// GET & SET ===========================================================================================================
bool LOTRI_GetModelVertex(const LOTRI_Model *model, const int index, Vec3f* vec) {
    REQ_CONDITION(model != NULL, return false);
    REQ_CONDITION(0 <= index && index < model->numVertices, return false);
    *vec = model->vertices[index].xyz;
    return true;
}
bool LOTRI_SetModelNormals(const LOTRI_Model* model, const ModelSide side) {
    REQ_CONDITION(model != NULL, return false;);
    if (side == MODEL_SIDE_NULL) return true;

    for (int i = 0; i < model->numFaces; i++) {
        const Vec3i index = model->faces[i].ijk;
        const Vec3f a = model->vertices[index.v.i].xyz;
        const Vec3f b = model->vertices[index.v.j].xyz;
        const Vec3f c = model->vertices[index.v.k].xyz;
        const Vec3f normal = BASIC_GetNormal(a, b, c);
        const Vec3f sum = BASIC_GetSum(a, b, c);
        if (side == MODEL_SIDE_IN ^ BASIC_GetDot(normal, sum) < 0) {
            model->faces[i].xyz = (Vec3f){-normal.v.x, -normal.v.y, -normal.v.z};
        }
        else {
            model->faces[i].xyz = normal;
        }
    }
    return true;
}


// CREATE & DELETE =====================================================================================================
static bool LOTRI_CreateModel_RK(LOTRI_Model* model, const fastObjMesh* mesh, const char* file_mtl, const ModelSide side) {
    model->numVertices = (int)mesh->position_count;
    model->vertices = calloc(model->numVertices, sizeof(LOTRI_Vertex));
    REQ_CONDITION(model->vertices != NULL, return false);

    for (int i = 0; i < model->numVertices; i++) {
        model->vertices[i].xyz = (Vec3f){
            mesh->positions[3*i],
            mesh->positions[3*i+1],
            mesh->positions[3*i+2]
        };
        model->vertices[i].rgba = (Vec4f){1, 1, 1, 1};
        model->vertices[i].uv = (Vec2f){
            mesh->texcoords[2*i],
            1-mesh->texcoords[2*i+1]
        };
    }

    model->numFaces = (int)mesh->face_count;
    model->faces = calloc(model->numFaces, sizeof(LOTRI_Face));
    REQ_CONDITION(model->faces != NULL, return false);

    for (int i = 0; i < model->numFaces; i++) {
        model->faces[i].ijk = (Vec3i){
            (int)mesh->indices[3*i].p,
            (int)mesh->indices[3*i+1].p,
            (int)mesh->indices[3*i+2].p,
        };
        model->faces[i].xyz = (Vec3f){
            mesh->normals[3*i+3],
            mesh->normals[3*i+4],
            mesh->normals[3*i+5],
        };
    }

    model->texture = IMG_LoadTexture(renderer, file_mtl);
    REQ_CONDITION(model->texture != NULL, return false);
    SDL_SetTextureScaleMode(model->texture, SDL_SCALEMODE_NEAREST);

    model->side = side;
    LOTRI_SetModelNormals(model, side);

    return true;
}
LOTRI_Model* LOTRI_CreateModel(const char* file_obj, const char* file_mtl, const ModelSide side) {

    LOTRI_Model* model = calloc(1, sizeof(LOTRI_Model));
    REQ_CONDITION(model != NULL, return NULL);

    fastObjMesh* mesh = fast_obj_read(file_obj);
    REQ_CONDITION(mesh != NULL, {
        model = LOTRI_DeleteModel(model);
        return NULL;
    });

    const bool RK = LOTRI_CreateModel_RK(model, mesh, file_mtl, side);
    fast_obj_destroy(mesh);
    mesh = NULL;
    REQ_CONDITION(RK, {
        model = LOTRI_DeleteModel(model);
        return NULL;
    });

    return model;
}
LOTRI_Model* LOTRI_DeleteModel(LOTRI_Model* model) {
    if (model == NULL) return model;
    if (model->vertices != NULL) {
        free(model->vertices);
        model->vertices = NULL;
    }
    if (model->texture != NULL) {
        SDL_DestroyTexture(model->texture);
        model->texture = NULL;
    }
    if (model->faces != NULL) {
        free(model->faces);
        model->faces = NULL;
    }
    free(model);
    model = NULL;
    return model;
}

