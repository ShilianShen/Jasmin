#ifndef TABLE_H
#define TABLE_H


#include "_basic.h"


typedef struct KeyVal {char* key; void* val;} KeyVal;
typedef struct Table {
    int len;
    KeyVal* kv;
} Table;


void* TABLE_GetValByKey(Table table, const char* key);
const char* TABLE_GetKeyByVal(Table table, const void* val);

bool BASIC_CreateTable(Table* table, const cJSON* table_json, void* (*create)(const cJSON*));
void BASIC_DestroyTable(Table* table, void (*destroy)(void*));


#endif //TABLE_H