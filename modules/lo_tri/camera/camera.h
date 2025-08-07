#ifndef JASMIN_LOTRI_CAMERA_H
#define JASMIN_LOTRI_CAMERA_H


#include "../math/math.h"


typedef struct Camera {
    Vec3f translation;
    Vec3f rotation;
} Camera;


extern Camera camera;


void LOTRI_RenewCamera();


#endif //JASMIN_LOTRI_CAMERA_H