#ifndef CAMERA_H
#define CAMERA_H


#include "matrix.h"


typedef struct Camera {
    Vec3f position;
    Vec3f rotation;
    Vec3f scale;
    Mat4f mat;
} Camera;

extern Camera camera;


bool LOTRI_RenewCamera();


#endif //CAMERA_H