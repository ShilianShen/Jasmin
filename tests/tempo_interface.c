#include "../include/jasmin/jasmin.h"


int test_int = 10;
float test_float = 10.5f;
bool test_bool = true;


const KeyVal TEMPO_PUBLIC_INT[] = {
    {"test_int", &test_int}
};
const int TEMPO_PUBLIC_INT_LEN = sizeof(TEMPO_PUBLIC_INT) / sizeof(KeyVal);

const KeyVal TEMPO_PUBLIC_FLOAT[] = {
    {"test_float", &test_float},
};
const int TEMPO_PUBLIC_FLOAT_LEN = sizeof(TEMPO_PUBLIC_FLOAT) / sizeof(KeyVal);

const KeyVal TEMPO_PUBLIC_BOOL[] = {
    {"test_bool", &test_bool},
};
const int TEMPO_PUBLIC_BOOL_LEN = sizeof(TEMPO_PUBLIC_BOOL) / sizeof(KeyVal);


const Table INT_EE[JSM_NUM_TYPES] = {
    [JSM_INT] = {sizeof(TEMPO_PUBLIC_INT) / sizeof(KeyVal), TEMPO_PUBLIC_INT},
    [JSM_FLOAT] = {sizeof(TEMPO_PUBLIC_FLOAT) / sizeof(KeyVal), TEMPO_PUBLIC_FLOAT},
    [JSM_BOOL] = {sizeof(TEMPO_PUBLIC_BOOL) / sizeof(KeyVal), TEMPO_PUBLIC_BOOL},
};