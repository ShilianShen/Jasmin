#ifndef BASIC_TABLE_H
#define BASIC_TABLE_H


#include "_basic.h"


typedef struct KeyVal {char* key; void* val;} KeyVal;
typedef struct Table {int len; KeyVal* kv;} Table;


void* BASIC_GetTableValByKey(Table table, const char* key);
const char* BASIC_GetTableKeyByVal(Table table, const void* val);
void* BASIC_GetTableValByIdx(Table table, int idx);
const char* BASIC_GetTableKeyByIdx(Table table, int idx);
bool BASIC_SortTableByKey(Table table, const char* keys[]);
bool BASIC_GetTableIdxByKey(Table table, const char* key, int* idx);
bool BASIC_GetTableIdxByVal(Table table, const void* val, int* idx);


bool BASIC_CreateTable(Table* table, void *func, const cJSON* table_json);
bool BASIC_RenewTable(const Table* table, void *func);
bool BASIC_DrawTable(const Table* table, void *func);
void BASIC_DeleteTable(Table* table, void *func);
void BASIC_PrintTable(const Table* table);


#endif //BASIC_TABLE_H