#ifndef JASMIN_MENU_INTERFACE_H
#define JASMIN_MENU_INTERFACE_H


#include "../perph/perph.h"
#define TEMPO_JSON "../config/tempo.json"


extern const Table TEMPO_OUTER_TRIG_FUNC_TABLE;
extern const Table TEMPO_PTR_TABLE;


typedef struct {
    void* info;
    bool (*renew)(void*, SDL_Texture**, SDL_FPoint);
} TEMPO_TypeManu;


bool TEMPO_Init();
void TEMPO_Exit();
bool TEMPO_Renew();
bool TEMPO_Draw();


#endif //JASMIN_MENU_INTERFACE_H