#ifndef JASMIN_LOTRI_PIPLINE_H
#define JASMIN_LOTRI_PIPLINE_H


#include "../model/model.h"
#include "../camera/camera.h"
#include "../zbuffer/zbuffer.h"


void PIPLINE_Init();
void PIPLINE_Deinit();

void PIPLINE_Load();
void PIPLINE_Unload();

void PIPLINE_Draw();
void PIPLINE_Renew();


#endif //JASMIN_LOTRI_PIPLINE_H