#include "table.h"


int* BASIC_GetIntFromTable(const KeyValI table[], const char* key) {
    if (table == NULL) {
        return NULL;
    }
    for (int i = 0; table[i].key != NULL; i++) {
        if (strcmp(key, table[i].key) == 0) {
            return table[i].value;
        }
    }
    return NULL;
}
float* BASIC_GetFloatFromTable(const KeyValF table[], const char* key) {
    if (table == NULL) {
        return NULL;
    }
    for (int i = 0; table[i].key != NULL; i++) {
        if (strcmp(key, table[i].key) == 0) {
            return table[i].value;
        }
    }
    return NULL;
}