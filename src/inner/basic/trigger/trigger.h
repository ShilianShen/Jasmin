#ifndef TRIG_H
#define TRIG_H

#include "../setting/setting.h"
typedef void (*TrigFunc)(char*);
struct Trig {
    TrigFunc func;
    char* para;
};
typedef struct Trig Trig;


Trig* TEMPO_DeleteTrig(Trig* trig);
Trig* TEMPO_CreateTrig(const toml_table_t* tomlTrig);


#endif //TRIG_H