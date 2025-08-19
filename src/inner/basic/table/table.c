#include "table.h"


void* BASIC_GetValByKey(const KeyVal table[], const char* key) {
    for (int idx = 0; table[idx].key != NULL; idx++) {
        if (strcmp(key, table[idx].key) == 0) {
            return table[idx].val;
        }
    }
    return NULL;
}
const char* BASIC_GetKeyByVal(const KeyVal table[], const void* val) {
    for (int idx = 0; table[idx].key != NULL; idx++) {
        if (table[idx].val == val) {
            return table[idx].key;
        }
    }
    return NULL;
}