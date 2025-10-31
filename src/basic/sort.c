#include "sort.h"


typedef struct {int index; float key;} KeyIndex;
static int compare_key_index(const void *a, const void *b) {
    const float diff = ((KeyIndex*)a)->key - ((KeyIndex*)b)->key;
    if (diff < 0) return -1;
    if (diff > 0) return 1;
    return 0;
}
bool BASIC_SortIndices(const int N, const float keys[N], int indices[N], const bool ascending) {
    KeyIndex temp[N];
    for (int i = 0; i < N; ++i) {
        temp[i].index = i;
        temp[i].key = keys[i];
    }

    qsort(temp, N, sizeof(KeyIndex), compare_key_index);

    for (int i = 0; i < N; i++) {
        indices[i] = ascending ? temp[i].index : temp[N-1-i].index;
    }
    return true;
}