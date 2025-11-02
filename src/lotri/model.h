#ifndef MODEL_H
#define MODEL_H


#include "_lotri.h"
#include "camera.h"


typedef struct {Vec3f xyz; Vec4f rgba; Vec2f uv;} LOTRI_Vertex;
typedef struct {Vec3i ijk; Vec3f xyz;} LOTRI_Face;


typedef enum ModelSide {
    MODEL_SIDE_NULL,
    MODEL_SIDE_OUT,
    MODEL_SIDE_IN,
    MODEL_SIDE_CAMERA
} ModelSide;
typedef struct LOTRI_MW LOTRI_MW;
typedef struct LOTRI_Model LOTRI_Model;
typedef struct LOTRI_World LOTRI_World;
struct LOTRI_MW {
    LOTRI_Model* model;
    LOTRI_World* world;
};
struct LOTRI_Model {
    int numVertices;
    LOTRI_Vertex *vertices;
    int numFaces;
    LOTRI_Face *faces;
    ModelSide side;
    SDL_Texture* texture;
};
struct LOTRI_World {
    Vec3f scale, position, rotation;
    int numVertices, numFaces;
    LOTRI_Vertex *vertices;
    LOTRI_Face *faces;
    SDL_FRect* src;
    float depth;
};


LOTRI_Model* LOTRI_DeleteModel(LOTRI_Model* model);




bool LOTRI_GetModelVertex(const LOTRI_Model *model, int index, Vec3f* vec);


LOTRI_Model* LOTRI_CreateModel(const fastObjMesh* mesh, const char* file_mtl, ModelSide side);
LOTRI_World* LOTRI_CreateWorld(const LOTRI_Model* model);



bool LOTRI_RenewModel(LOTRI_MW* mw);


bool LOTRI_DrawModel(const LOTRI_MW* mw);



#endif //MODEL_H