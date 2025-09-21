#include "table.h"


// SET & GET ===========================================================================================================
void* TABLE_GetValByKey(const Table table, const char* key) {
    if (key == NULL) return NULL;

    for (int idx = 0; idx < table.len; idx++) {
        if (strcmp(key, table.kv[idx].key) == 0) {
            return table.kv[idx].val;
        }
    }
    return NULL;
}
const char* TABLE_GetKeyByVal(const Table table, const void* val) {
    for (int idx = 0; idx < table.len; idx++) {
        if (table.kv[idx].val == val) {
            return table.kv[idx].key;
        }
    }
    return NULL;
}


// CREATE & DELETE =====================================================================================================
bool BASIC_CreateTable(Table* table, const cJSON* table_json, const CreateFunc func) {
    if (table_json == NULL) {
        printf("%s: table_json == NULL.\n", __func__);
        return false;
    }

    table->len = cJSON_GetArraySize(table_json);
    if (table->len <= 0) {
        printf("%s: table->len <= 0.\n", __func__);
        return false;
    }

    table->kv = calloc(table->len, sizeof(KeyVal));
    if (table->kv == NULL) {printf("%s: table->kv == NULL.\n", __func__); return false;}

    for (int i = 0; i < table->len; i++) {
        const cJSON* keyval_json = cJSON_GetArrayItem(table_json, i);

        table->kv[i].key = strdup(keyval_json->string);
        if (table->kv[i].key == NULL) {printf("%s: table->kv[i].key == NULL.\n", __func__); return false;}

        table->kv[i].val = func(keyval_json);
        if (table->kv[i].val == NULL) {printf("%s: table->kv[i].val == NULL.\n", __func__); return false;}
    }
    return true;
}
void BASIC_DeleteTable(Table* table, const DestroyFunc func) {
    if (table->kv != NULL) {
        for (int i = 0; i < table->len; i++) {
            if (table->kv[i].key != NULL) {
                free(table->kv[i].key);
                table->kv[i].key = NULL;
            }

            if (table->kv[i].val != NULL) {
                func(table->kv[i].val);
                table->kv[i].val = NULL;
            }
        }
        free(table->kv);
        table->kv = NULL;
    }

    table->len = 0;
}

