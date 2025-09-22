#ifndef BASIC_ENV_H
#define BASIC_ENV_H


#include "_basic.h"


typedef struct Env {
    const char* name;
    bool (*init)();
    bool (*renew)();
    bool (*draw)();
    void (*exit)();
} Env;


bool BASIC_InitEnv(int len, const Env arr[len]);
bool BASIC_RenewEnv(int len, const Env arr[len]);
bool BASIC_DrawEnv(int len, const Env arr[len]);
void BASIC_ExitEnv(int len, const Env arr[len]);


#endif //BASIC_ENV_H