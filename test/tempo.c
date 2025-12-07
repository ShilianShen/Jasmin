#include "../../Jasmin/include/jasmin/jasmin.h"


int test_int = 10;
float test_float = 10.5f;
bool test_bool = true;
///ftufti
char test_string[] = "test_string!!!";


static KeyVal aaa[] = {
    {"test_int", &test_int},
    {"test_float", &test_float},
    {"test_bool", &test_bool},
    {"test_string", &test_string},
};
const Table TEMPO_PTR_TABLE = {.kv = aaa, .len = len_of(aaa)};


static KeyVal tt[] = {
};
const Table TEMPO_OUTER_TRIG_FUNC_TABLE = {.kv = tt, .len = len_of(tt)};