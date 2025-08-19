#ifndef JASMIN_MENU_STRUCT_TRIG_H
#define JASMIN_MENU_STRUCT_TRIG_H


// #include "../basic/basic.h"
#include "../interface.h"


KeyVal TEMPO_MENU_TRIG_SET[TRIG_NUM_TYPES];
void TEMPO_TrigFuncPass(TrigPara);
void TEMPO_TrigFuncForward(TrigPara);
void TEMPO_TrigFuncBackward(TrigPara);
void TEMPO_TrigFuncClear(TrigPara);
void TEMPO_TrigFuncKnob(TrigPara);


Trig* TEMPO_DeleteTrig(Trig* trig);
Trig* TEMPO_CreateTrig(const toml_table_t* tomlTrig);


#endif //JASMIN_MENU_STRUCT_TRIG_H
