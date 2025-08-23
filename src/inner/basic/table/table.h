#ifndef TABLE_H
#define TABLE_H


#include "../setting/setting.h"


typedef struct KeyVal {char* key; void* val;} KeyVal;


void* BASIC_GetValByKey(const KeyVal table[], const char* key);
const char* BASIC_GetKeyByVal(const KeyVal table[], const void* val);


#endif //TABLE_H