#include "intel.h"


const char* INTEL_STATE_STRING[INTEL_NUM_STATES] = {
    [INTEL_STATE_NULL] = "NULL",
    [INTEL_STATE_SRC_FALSE] = "SRC_FALSE",
    [INTEL_STATE_SRC_TRUE] = "SRC_TRUE",
    [INTEL_STATE_UNKNOWN] = "UNKNOWN",
};
IntelNet* testIntelNet = NULL;


bool compute_force_layout(
    const int N, const Intel intelSet[N],
    const int M, SDL_FPoint positions[M],
    const SDL_FRect bckRect
) {
    static const int MAX_ITER = 200;      // 迭代次

    REQ_CONDITION(N > 0 && intelSet != NULL && M > 0 && positions != NULL, return false);

    // 初始化节点随机位置
    // SDL_srand(25);
    // for (int i = 0; i < M; i++) {
    //     positions[i].x = bckRect.x + SDL_randf() * bckRect.w;
    //     positions[i].y = bckRect.y + SDL_randf() * bckRect.h;
    // }

    // 暂存力向量
    // for (int iter = 0; iter < MAX_ITER; iter++)
    {
        SDL_FPoint forces[M];
        for (int i = 0; i < M; i++) {
            forces[i].x = 0;
            forces[i].y = 0;
        }

        // 计算斥力
        for (int i = 0; i < M; i++) {
            for (int j = i + 1; j < M; j++) {
                const float dx = positions[i].x - positions[j].x;
                const float dy = positions[i].y - positions[j].y;
                const float dist2 = dx * dx + dy * dy + 0.01f;
                const float dist = sqrtf(dist2);
                const float force = 2000 / dist2;

                const float fx = force * dx / dist;
                const float fy = force * dy / dist;

                forces[i].x += fx; forces[i].y += fy;
                forces[j].x -= fx; forces[j].y -= fy;
            }
        }

        // 计算引力
        for (int r = 0; r < N; r++) {
            const int a = intelSet[r].subject;
            const int b = intelSet[r].object;
            if (a < 0 || a >= M || b < 0 || b >= M) continue;

            const float dx = positions[a].x - positions[b].x;
            const float dy = positions[a].y - positions[b].y;
            const float dist = sqrtf(dx * dx + dy * dy) + 0.01f;
            const float force = 0.05f * dist;
            const float fx = force * dx / dist;
            const float fy = force * dy / dist;

            forces[a].x -= fx;
            forces[a].y -= fy;
            forces[b].x += fx;
            forces[b].y += fy;
        }

        // 更新位置
        static const float DAMPING = 0.85f;     // 阻尼系数
        for (int i = 0; i < M; i++) {
            positions[i].x += forces[i].x * DAMPING;
            positions[i].y += forces[i].y * DAMPING;

            if (positions[i].x < bckRect.x) positions[i].x = bckRect.x;
            if (positions[i].x > bckRect.x + bckRect.w) positions[i].x = bckRect.x + bckRect.w;
            if (positions[i].y < bckRect.y) positions[i].y = bckRect.y;
            if (positions[i].y > bckRect.y + bckRect.h) positions[i].y = bckRect.y + bckRect.h;
        }
    }
    return true;
}


static Intel* INTEL_GetIntelSet(const int len) {
    REQ_CONDITION(len > 0, return NULL);

    Intel* intelSet = calloc(len, sizeof(Intel));
    REQ_CONDITION(intelSet != NULL, return NULL);

    return intelSet;
}
static bool INTEL_CreateIntelNet_RK(IntelNet* intelNet) {
    memset(intelNet, 0, sizeof(IntelNet));

    intelNet->len = 10;
    intelNet->intelSet = INTEL_GetIntelSet(intelNet->len);
    REQ_CONDITION(intelNet->intelSet != NULL, return false);

    return true;
}
IntelNet* INTEL_CreateIntelNet() {
    IntelNet* intelNet = malloc(sizeof(IntelNet));
    REQ_CONDITION(intelNet != NULL, return NULL);
    REQ_CONDITION(INTEL_CreateIntelNet_RK(intelNet), intelNet = INTEL_DeleteIntelNet(intelNet));
    return intelNet;
}
IntelNet* INTEL_DeleteIntelNet(IntelNet* intelNet) {
    if (intelNet != NULL) {
        if (intelNet->intelSet != NULL) {
            free(intelNet->intelSet);
            intelNet->intelSet = NULL;
        }
        free(intelNet);
    }
    intelNet = NULL;
    return NULL;
}
bool INTEL_AppendIntelNet(IntelNet* intelNet, const Intel intel) {
    REQ_CONDITION(intelNet->intelSet != NULL, return false);

    for (int i = 0; i < intelNet->len; i++) {
        if (intelNet->intelSet[i].state == INTEL_STATE_NULL) {
            intelNet->intelSet[i] = intel;
            return true;
        }
    }
    const int len = (int)(intelNet->len * 1.5);
    Intel* intelSet = INTEL_GetIntelSet(len);
    REQ_CONDITION(intelSet != NULL, return false);

    for (int i = 0; i < intelNet->len; i++) {
        intelSet[i] = intelNet->intelSet[i];
    }
    intelSet[intelNet->len] = intel;

    free(intelNet->intelSet);
    intelNet->intelSet = intelSet;
    intelNet->len = len;


    return true;
}
bool INTEL_DrawIntelNet(IntelNet* intelNet) {
    REQ_CONDITION(intelNet->intelSet != NULL, return false);

    int n = 0;
    for (int i = 0; i < intelNet->len; i++) {
        n = SDL_max(intelNet->intelSet[i].subject, n);
        n = SDL_max(intelNet->intelSet[i].object, n);
    }
    n++;
    SDL_FPoint points[n];
    compute_force_layout(
        intelNet->len, intelNet->intelSet,
        n, points, (SDL_FRect){0, 0, (float)windowWidth, (float)windowHeight}
        );
    SDL_FRect rects[n];
    for (int i = 0; i < n; i++) {
        rects[i] = (SDL_FRect){points[i].x, points[i].y, 20, 20};
    }
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderRects(renderer, rects, n);
    SDL_RenderRect(renderer, &(SDL_FRect){0, 0, (float)windowWidth, (float)windowHeight});
    return true;
}


const char* tabData[] = {
    "0", "1", "2"
};




bool INTEL_Init() {
    testIntelNet = INTEL_CreateIntelNet();
    INTEL_AppendIntelNet(testIntelNet, (Intel){INTEL_STATE_SRC_TRUE, 0, 1, 1});
    return true;
}
bool INTEL_Renew() {
    return true;
}
bool INTEL_Draw() {
    INTEL_DrawIntelNet(testIntelNet);

    return true;
}
void INTEL_Exit() {
    testIntelNet = INTEL_DeleteIntelNet(testIntelNet);
}