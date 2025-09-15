#ifndef MODEL_H
#define MODEL_H


#include "matrix.h"


typedef struct Model Model;
struct Model {
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



Model* LOTRI_CreateModel(int numVertices, int numFaces);
void LOTRI_DestroyModel(Model* model);


bool LOTRI_SetModelNormals(const Model* model, bool in);
bool LOTRI_SetModelVertices(const Model* model, int N, const Vec3f xyz[N]);
bool LOTRI_SetModelFaces(const Model* model, int N, const Vec3i abc[N]);
bool LOTRI_SetModelTexture(Model* model, const char* filename);
bool LOTRI_SetModelUV(const Model* model, int N, const Vec2f uv[N]);


bool LOTRI_RenewModel(Model* model);
bool LOTRI_DrawModel(const Model* model);


#endif //MODEL_H