#ifndef TABLE_H
#define TABLE_H


#include "../func/func.h"


typedef struct KeyVal {const char* key; void* val;} KeyVal;


void* BASIC_GetValByKey(const KeyVal table[], const char* key);
const char* BASIC_GetKeyByVal(const KeyVal table[], const void* val);


#endif //TABLE_H