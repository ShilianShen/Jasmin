#ifndef BASIC_TABLE_H
#define BASIC_TABLE_H


#include "_basic.h"


typedef struct KeyVal {char* key; void* val;} KeyVal;
typedef struct Table {int len; KeyVal* kv;} Table;


typedef void*(*CreateFunc)(const cJSON*);
typedef bool (*RenewFunc)(void*);
typedef bool (*DrawFunc)(const void*);
typedef void*(*DeleteFunc)(void*);


void* BASIC_GetTableValByKey(Table table, const char* key);
const char* BASIC_GetTableKeyByVal(Table table, const void* val);
void* BASIC_GetTableValByIdx(Table table, int idx);
const char* BASIC_GetTableKeyByIdx(Table table, int idx);
bool BASIC_SortTableByKey(Table table, const char* keys[]);
bool BASIC_GetTableIdxByKey(Table table, const char* key, int* idx);
bool BASIC_GetTableIdxByVal(Table table, const void* val, int* idx);


bool BASIC_CreateTable(Table* table, const cJSON* table_json, CreateFunc func);
bool BASIC_RenewTable(const Table* table, RenewFunc func);
bool BASIC_DrawTable(const Table* table, DrawFunc func);
void BASIC_DeleteTable(Table* table, DeleteFunc func);
void BASIC_PrintTable(const Table* table);


#endif //BASIC_TABLE_H