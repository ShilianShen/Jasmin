#include "../include/jasmin/jasmin.h"

#define USE_TEMPO_EXTERN_TABLE

int test_int = 10;
float test_float = 10.5f;
bool test_bool = true;


static const KeyVal TABLE_INT[] = {
    {"test_int", &test_int}
};

static const KeyVal TABLE_FLOAT[] = {
    {"test_float", &test_float},
};

static const KeyVal TABLE_BOOL[] = {
    {"test_bool", &test_bool},
};


const Table TEMPO_ExternTable[] = {
    [JSM_INT] = {sizeof(TABLE_INT) / sizeof(KeyVal), TABLE_INT},
    [JSM_FLOAT] = {sizeof(TABLE_FLOAT) / sizeof(KeyVal), TABLE_FLOAT},
    [JSM_BOOL] = {sizeof(TABLE_BOOL) / sizeof(KeyVal), TABLE_BOOL},
};
