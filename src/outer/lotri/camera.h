#ifndef CAMERA_H
#define CAMERA_H


#include "matrix.h"
#include "model.h"


typedef struct Camera {
    Vec3f position;
    Vec3f rotation, rotation1, rotation2;
    Vec3f scale;
    Mat4f mat;
} Camera;

extern Camera camera;


bool LOTRI_RenewCamera();


#endif //CAMERA_H