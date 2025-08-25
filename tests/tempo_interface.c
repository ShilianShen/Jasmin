#include "../include/jasmin/jasmin.h"


int test_int = 10;
float test_float = 10.5f;
bool test_bool = true;


const KeyVal TEMPO_TABLE_INT[] = {
    {"test_int", &test_int},
    {NULL, NULL}
};
const KeyVal TEMPO_TABLE_FLOAT[] = {
    {"test_float", &test_float},
    {NULL, NULL}
};
const KeyVal TEMPO_TABLE_BOOL[] = {
    {"test_bool", &test_bool},
    {NULL, NULL}
};