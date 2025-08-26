#ifndef TABLE_H
#define TABLE_H


#include "setting.h"


typedef struct KeyVal {char* key; void* val;} KeyVal;


void* BASIC_GetValByKey(int len, const KeyVal table[], const char* key);
const char* BASIC_GetKeyByVal(int len, const KeyVal table[], const void* val);


#endif //TABLE_H