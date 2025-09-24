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


// CREATE & DELETE =====================================================================================================
bool BASIC_CreateTable(Table* table, const cJSON* table_json, const CreateFunc func) {
    REQ_CONDITION(table_json != NULL, return false);

    table->len = cJSON_GetArraySize(table_json);
    REQ_CONDITION(table->len > 0, return false);

    table->kv = calloc(table->len, sizeof(KeyVal));
    REQ_CONDITION(table->kv != NULL, return false);

    for (int i = 0; i < table->len; i++) {
        const cJSON* kv_json = cJSON_GetArrayItem(table_json, i);
        REQ_CONDITION(kv_json != NULL, return false);

        table->kv[i].key = strdup(kv_json->string);
        REQ_CONDITION(table->kv[i].key != NULL, return false);

        table->kv[i].val = func(kv_json);
        REQ_CONDITION(table->kv[i].val != NULL, return false);
    }
    return true;
}
bool BASIC_RenewTable(const Table* table, const RenewFunc func) {
    if (table == NULL || func == NULL) {
        printf("%s: table == NULL || func == NULL.\n", __func__);
        return false;
    }

    for (int i = 0; i < table->len; i++) {
        if (func(table->kv[i].val) == false) {
            printf("%s: func(table->kv[i].val) == false.\n", __func__);
            return false;
        }
    }

    return true;
}
bool BASIC_DrawTable(const Table* table, const DrawFunc func) {
    if (table == NULL || func == NULL) {
        printf("%s: table == NULL || func == NULL.\n", __func__);
        return false;
    }

    for (int i = 0; i < table->len; i++) {
        if (func(table->kv[i].val) == false) {
            printf("%s: func(table->kv[i].val) == false.\n", __func__);
            return false;
        }
    }

    return true;
}
void BASIC_DeleteTable(Table* table, const DeleteFunc func) {
    if (table->kv != NULL) {
        for (int i = 0; i < table->len; i++) {
            if (table->kv[i].key != NULL) {
                free(table->kv[i].key);
                table->kv[i].key = NULL;
            }

            if (table->kv[i].val != NULL) {
                table->kv[i].val = func(table->kv[i].val);
            }
        }
        free(table->kv);
        table->kv = NULL;
    }

    table->len = 0;
}
void BASIC_PrintTable(const Table* table) {
    for (int i = 0; i < table->len; i++) {
        printf("%d, %s, %p\n", i, table->kv[i].key, table->kv[i].val);
    }
}

