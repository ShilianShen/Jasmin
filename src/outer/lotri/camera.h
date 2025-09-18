#ifndef CAMERA_H
#define CAMERA_H


#include "matrix.h"
#include "model.h"

typedef struct Camera Camera;
struct Camera {
    Vec3f position;
    Vec3f rotation, rotation1, rotation2;
    Vec3f scale;
    Mat4f mat;
};
extern Camera camera;


bool LOTRI_SetCameraRotation(Vec3f rotation);
bool LOTRI_RenewCamera();


#endif //CAMERA_H