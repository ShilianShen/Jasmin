#include "../include/jasmin/jasmin.h"


int test_int = 10;
float test_float = 10.5f;
bool test_bool = true;
char test_string[] = "test_string!!!";


static KeyVal TABLE_INT[] = {
    {"test_int", &test_int}
};

static KeyVal TABLE_FLOAT[] = {
    {"test_float", &test_float},
};

static KeyVal TABLE_BOOL[] = {
    {"test_bool", &test_bool},
};

static KeyVal TABLE_STRING[] = {
    {"test_string", &test_string},
};


const Table TEMPO_ExternTable[] = {
    [JSM_INT] = {sizeof(TABLE_INT) / sizeof(KeyVal), TABLE_INT},
    [JSM_FLOAT] = {sizeof(TABLE_FLOAT) / sizeof(KeyVal), TABLE_FLOAT},
    [JSM_BOOL] = {sizeof(TABLE_BOOL) / sizeof(KeyVal), TABLE_BOOL},
    [JSM_STRING] = {sizeof(TABLE_STRING) / sizeof(KeyVal), TABLE_STRING},
};
Table TEMPO_ExternPageTable = {0, NULL};