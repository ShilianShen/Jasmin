#ifndef JASMIN_MENU_STRUCT_TRIG_H
#define JASMIN_MENU_STRUCT_TRIG_H


#include "../basic/basic.h"


// datatype
union TrigPara {
    char* pageName;
    struct {
        float min, max, now;
    } knob;
    struct {
        int min, max, step, now;
    } slider;
};
enum TrigType {
    TRIG_TYPE_PASS,
    TRIG_TYPE_FORWARD,
    TRIG_TYPE_BACKWARD,
    TRIG_TYPE_CLEAR,
    TRIG_TYPE_KNOB,
    TRIG_NUM_TYPES
};
typedef enum TrigType TrigType;
typedef union TrigPara TrigPara;
typedef void (*TrigFunc)(TrigPara);
struct Trig {
    TrigType type;
    TrigFunc func;
    TrigPara para;
};
typedef struct Trig Trig;


void TEMPO_TrigFuncPass(TrigPara);
void TEMPO_TrigFuncForward(TrigPara);
void TEMPO_TrigFuncBackward(TrigPara);
void TEMPO_TrigFuncClear(TrigPara);
void TEMPO_TrigFuncKnob(TrigPara);



TrigFunc TRIG_GetFuncFromName(const char* name);
const char* TRIG_GetNameFromFunc(TrigFunc func);

Trig* TEMPO_DeleteTrig(Trig* trig);
Trig* TEMPO_CreateTrig(const toml_table_t* tomlTrig);


#endif //JASMIN_MENU_STRUCT_TRIG_H
