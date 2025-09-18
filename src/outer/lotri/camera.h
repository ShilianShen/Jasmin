#ifndef CAMERA_H
#define CAMERA_H


#include "matrix.h"
#include "model.h"


typedef struct Camera Camera;
struct Camera {
    Vec3f position, position1, position2;
    Vec3f rotation, rotation1, rotation2;
    Vec3f scale;
    Mat4f mat;
    float t, t1, t2;
};
extern Camera camera;


bool LOTRI_SetCamera(Vec3f rotation, Vec3f position, float time);

bool LOTRI_RenewCamera();


#endif //CAMERA_H