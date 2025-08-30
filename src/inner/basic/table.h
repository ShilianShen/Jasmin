#ifndef TABLE_H
#define TABLE_H


#include "setting.h"


typedef struct KeyVal {char* key; void* val;} KeyVal;
typedef struct Table {int len; KeyVal* kv;} Table;


void* TABLE_GetValByKey(Table table, const char* key);
const char* TABLE_GetKeyByVal(Table table, const void* val);


#endif //TABLE_H