#ifndef ENV_H
#define ENV_H


#include "_basic.h"


typedef struct Env {
    const char* name;
    bool (*init)();
    bool (*renew)();
    bool (*draw)();
    void (*exit)();
} Env;


bool Init(int len, const Env arr[len]);
bool Renew(int len, const Env arr[len]);
bool Draw(int len, const Env arr[len]);
void Exit(int len, const Env arr[len]);


#endif //ENV_H