#ifndef CAMERA_H
#define CAMERA_H


#include "model.h"


typedef struct LOTRI_Camera LOTRI_Camera;
struct LOTRI_Camera {
    Vec3f position;
    Vec3f rotation;
    Vec3f scale;
    Mat4f mat;
};
extern LOTRI_Camera camera;


bool LOTRI_SetCamera(Vec3f rotation, Vec3f position);

bool LOTRI_RenewCamera();
bool LOTRI_DrawCamera();


#endif //CAMERA_H