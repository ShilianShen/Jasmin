#include "../include/jasmin/jasmin.h"


int test_int = 10;
float test_float = 10.5f;
bool test_bool = true;


const KeyVal TEMPO_TABLE_INT[] = {
    {"test_int", &test_int},
    {NULL, NULL}
};
const int TEMPO_TABLE_INT_LEN = sizeof(TEMPO_TABLE_INT) / sizeof(KeyVal);

const KeyVal TEMPO_TABLE_FLOAT[] = {
    {"test_float", &test_float},
    {NULL, NULL}
};
const int TEMPO_TABLE_FLOAT_LEN = sizeof(TEMPO_TABLE_FLOAT) / sizeof(KeyVal);

const KeyVal TEMPO_TABLE_BOOL[] = {
    {"test_bool", &test_bool},
    {NULL, NULL}
};
const int TEMPO_TABLE_BOOL_LEN = sizeof(TEMPO_TABLE_BOOL) / sizeof(KeyVal);