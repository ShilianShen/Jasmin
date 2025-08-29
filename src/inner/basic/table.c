#include "table.h"


static void* BASIC_GetValByKey(const int len, const KeyVal table[], const char* key) {
    for (int idx = 0; idx < len; idx++) {
        if (strcmp(key, table[idx].key) == 0) {
            return table[idx].val;
        }
    }
    return NULL;
}
static const char* BASIC_GetKeyByVal(const int len, const KeyVal table[], const void* val) {
    for (int idx = 0; idx < len; idx++) {
        if (table[idx].val == val) {
            return table[idx].key;
        }
    }
    return NULL;
}


void* TABLE_GetValByKey(const Table table, const char* key) {
    return BASIC_GetValByKey(table.len, table.kv, key);
}
const char* TABLE_GetKeyByVal(const Table table, const void* val) {
    return BASIC_GetKeyByVal(table.len, table.kv, val);
}