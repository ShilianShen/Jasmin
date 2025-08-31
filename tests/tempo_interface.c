#include "../include/jasmin/jasmin.h"

#define USE_TEMPO_EXTERN_TABLE

int test_int = 10;
float test_float = 10.5f;
bool test_bool = true;


static KeyVal TABLE_INT[] = {
    {"test_int", &test_int}
};

static KeyVal TABLE_FLOAT[] = {
    {"test_float", &test_float},
};

static KeyVal TABLE_BOOL[] = {
    {"test_bool", &test_bool},
};


const Table TEMPO_ExternTable[] = {
    [JSM_INT] = {sizeof(TABLE_INT) / sizeof(KeyVal), TABLE_INT},
    [JSM_FLOAT] = {sizeof(TABLE_FLOAT) / sizeof(KeyVal), TABLE_FLOAT},
    [JSM_BOOL] = {sizeof(TABLE_BOOL) / sizeof(KeyVal), TABLE_BOOL},
};
