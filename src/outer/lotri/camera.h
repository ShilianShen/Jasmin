#ifndef CAMERA_H
#define CAMERA_H


#include "model.h"


typedef struct Camera Camera;
struct Camera {
    Vec3f position;
    Vec3f rotation;
    Vec3f scale;
    Mat4f mat;
};
extern Camera camera;


bool LOTRI_SetCamera(Vec3f rotation);

bool LOTRI_RenewCamera();
bool LOTRI_DrawCamera();


#endif //CAMERA_H