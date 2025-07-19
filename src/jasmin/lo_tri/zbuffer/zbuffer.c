#include "zbuffer.h"


ZBuffer zBuffer;
void ZBUFFER_Init(SDL_Renderer* ren) {
    renderer = ren;
    zBuffer = (ZBuffer){0};
    zBuffer.numFaces = 100;
    zBuffer.head = 0;
    zBuffer.faces = malloc(zBuffer.numFaces * sizeof(Face*));
    zBuffer.zs = malloc(zBuffer.numFaces * sizeof(float));
    zBuffer.indices = malloc(zBuffer.numFaces * sizeof(int));
}
void ZBUFFER_Deinit() {
    free(zBuffer.faces);
    free(zBuffer.zs);
    free(zBuffer.indices);
    zBuffer.numFaces = 0;
    zBuffer.head = 0;
    zBuffer.faces = NULL;
    zBuffer.zs = NULL;
    zBuffer.indices = NULL;
}
void ZBUFFER_Renew() {
    zBuffer.head = 0;
}

// DRAW
static const float *global_arr; // 用于比较的全局变量
int compare(const void *a, const void *b) {
    int ia = *(const int *) a;
    int ib = *(const int *) b;
    if (global_arr[ia] > global_arr[ib]) return -1;
    if (global_arr[ia] < global_arr[ib]) return 1;
    return 0;
}
void sort_indices_desc(const int num, const float arr[num], int idx[num]) {
    // 初始化索引数组
    for (int i = 0; i < num; i++) {
        idx[i] = i;
    }

    // 设置全局变量并排序
    global_arr = arr;
    qsort(idx, num, sizeof(int), compare);
}


void ZBUFFER_SendFaces(const int num, Face faces[num]) {
    for (int i = 0; i < num; i++) {
        zBuffer.faces[zBuffer.head] = &faces[i];
        zBuffer.zs[zBuffer.head] = VEC_GetMax(faces[i].z);
        zBuffer.head += 1;
    }
}
void ZBUFFER_Draw() {
    sort_indices_desc(zBuffer.head, zBuffer.zs, zBuffer.indices);
    for (int i = 0; i < zBuffer.head; i++) {
        const int idx = zBuffer.indices[i];
        const Face* face = zBuffer.faces[i];
        if (face->on) {
            SDL_RenderGeometry(renderer, NULL, face->vertices, 3, NULL, 0);
        }
        // DEBUG_DrawFace(zBuffer.faces[idx]->vertices);
    }

    DEBUG_SendMessageR("\n");
    DEBUG_SendMessageL("%s: zBuffer.head: %d\n", __func__, zBuffer.head);
}