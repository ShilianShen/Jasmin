#ifndef TABLE_H
#define TABLE_H


#include "../func/func.h"


typedef struct KeyValI {const char* key; int* value;} KeyValI;
typedef struct KeyValF {const char* key; float* value;} KeyValF;


int* BASIC_GetIntFromTable(const KeyValI table[], const char* key);
float* BASIC_GetFloatFromTable(const KeyValF table[], const char* key);


#endif //TABLE_H