#ifndef TABLE_H
#define TABLE_H


#include "_basic.h"


typedef struct KeyVal {char* key; void* val;} KeyVal;
typedef struct Table {
    int len;
    KeyVal* kv;
} Table;

typedef void* (*CreateFunc)(const cJSON*);
typedef void (*DestroyFunc)(void*);



void* TABLE_GetValByKey(Table table, const char* key);
const char* TABLE_GetKeyByVal(Table table, const void* val);

bool BASIC_CreateTable(Table* table, const cJSON* table_json, CreateFunc func);
void BASIC_DeleteTable(Table* table, DestroyFunc func);


#endif //TABLE_H