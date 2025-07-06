#ifndef JASMIN_LOTRI_PIPLINE_H
#define JASMIN_LOTRI_PIPLINE_H


#include "../model/model.h"
#include "../camera/camera.h"


void PIPLINE_RenewView();
void PIPLINE_RenewProj();
Mat4x4f PIPLINE_GetTran(const Vec3f translation, const Vec3f rotation);
void PIPLINE_RenewModelVertices(const Model model);
void PIPLINE_Draw(const Model model);


#endif //JASMIN_LOTRI_PIPLINE_H