#include "table.h"


// SET & GET ===========================================================================================================
void* BASIC_GetTableValByKey(const Table table, const char* key) {
    if (key == NULL) return NULL;

    for (int idx = 0; idx < table.len; idx++) {
        if (strcmp(key, table.kv[idx].key) == 0) {
            return table.kv[idx].val;
        }
    }
    return NULL;
}
const char* BASIC_GetTableKeyByVal(const Table table, const void* val) {
    for (int idx = 0; idx < table.len; idx++) {
        if (table.kv[idx].val == val) {
            return table.kv[idx].key;
        }
    }
    return NULL;
}
void* BASIC_GetTableValByIdx(const Table table, const int idx) {
    if (idx < 0 || idx >= table.len) return NULL;
    return table.kv[idx].val;
}
const char* BASIC_GetTableKeyByIdx(const Table table, const int idx) {
    if (idx < 0 || idx >= table.len) return NULL;
    return table.kv[idx].key;
}
bool BASIC_SortTableByKey(const Table table, const char* keys[]) {
    if (keys == NULL) return false;
    for (int i = 0; i < table.len; i++) {
        if (table.kv[i].key == NULL) continue;
        if (keys[i] == NULL) continue;
        for (int j = 0; j < table.len; j++) {
            if (table.kv[j].key == NULL) continue;
            if (i == j) continue;
            if (strcmp(keys[i], table.kv[j].key) == 0) {
                const KeyVal kv = table.kv[i];
                table.kv[i] = table.kv[j];
                table.kv[j] = kv;
            }
        }
    }
    return true;
}
bool BASIC_GetTableIdxByKey(const Table table, const char* key, int* idx) {
    if (key == NULL) return false;
    if (idx == NULL) return false;
    for (int i = 0; i < table.len; i++) {
        if (strcmp(key, table.kv[i].key) == 0) {
            *idx = i;
            return true;
        }
    }
    return false;
}
bool BASIC_GetTableIdxByVal(const Table table, const void* val, int* idx) {
    if (val == NULL) return false;
    if (idx == NULL) return false;
    for (int i = 0; i < table.len; i++) {
        if (val == table.kv[i].val) {
            *idx = i;
            return true;
        }
    }
    return false;
}


// CREATE & DELETE =====================================================================================================
bool BASIC_CreateTable(Table* table, void* func, const cJSON* table_json) {
    REQ_CONDITION(table_json != NULL, return false);
    REQ_CONDITION(cJSON_IsArray(table_json) || cJSON_IsObject(table_json), return false);
    table->len = cJSON_GetArraySize(table_json);
    REQ_CONDITION(table->len > 0, return false);

    table->kv = calloc(table->len, sizeof(KeyVal));
    REQ_CONDITION(table->kv != NULL, return false);

    for (int i = 0; i < table->len; i++) {
        const cJSON* kv_json = cJSON_GetArrayItem(table_json, i);
        REQ_CONDITION(kv_json != NULL, return false);

        if (kv_json->string != NULL) {
            table->kv[i].key = strdup(kv_json->string);
            REQ_CONDITION(table->kv[i].key != NULL, return false);
        }

        void*(*CreateFunc)(const cJSON*) = func;
        table->kv[i].val = CreateFunc(kv_json);
        REQ_CONDITION(table->kv[i].val != NULL, return false);
    }
    return true;
}
void BASIC_DeleteTable(Table* table, void* func) {
    void*(*deleteFunc)(void*) = func;
    if (table->kv != NULL) {
        for (int i = 0; i < table->len; i++) {
            if (table->kv[i].key != NULL) {
                free(table->kv[i].key);
                table->kv[i].key = NULL;
            }

            if (table->kv[i].val != NULL) {
                table->kv[i].val = deleteFunc(table->kv[i].val);
            }
        }
        free(table->kv);
        table->kv = NULL;
    }

    table->len = 0;
}
bool BASIC_RenewTable(const Table* table, void *func) {
    REQ_CONDITION(table != NULL && func != NULL, return false);

    bool (*renewFunc)(void*) = func;
    for (int i = 0; i < table->len; i++) {
        REQ_CONDITION(renewFunc(table->kv[i].val), return false);
    }

    return true;
}
bool BASIC_DrawTable(const Table* table, void *func) {
    REQ_CONDITION(table != NULL && func != NULL, return false);

    bool (*drawFunc)(const void*) = func;
    for (int i = 0; i < table->len; i++) {
        REQ_CONDITION(drawFunc(table->kv[i].val), return false);
    }

    return true;
}
void BASIC_PrintTable(const Table* table) {
    for (int i = 0; i < table->len; i++) {
        printf("%d, %s, %p\n", i, table->kv[i].key, table->kv[i].val);
    }
}

