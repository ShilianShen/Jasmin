#include "intel.h"

#include "../debug/debug.h"


const char* INTEL_STATE_STRING[INTEL_NUM_STATES] = {
    [INTEL_STATE_NULL] = "NULL",
    [INTEL_STATE_SRC_FALSE] = "SRC_FALSE",
    [INTEL_STATE_SRC_TRUE] = "SRC_TRUE",
    [INTEL_STATE_UNKNOWN] = "UNKNOWN",
};
IntelNet* testIntelNet = NULL;


enum EntityId {
    ENTITY_0,
    ENTITY_1,
    ENTITY_2,
    ENTITY_3,
    NUM_ENTITIES
};
typedef struct {
    const char* name;
    SDL_FPoint position;
    bool visible;
} Entity;


Entity entitySet[NUM_ENTITIES] = {
    [ENTITY_0] = {.name = "0000"},
    [ENTITY_1] = {.name = "1111"},
    [ENTITY_2] = {.name = "2222"},
    [ENTITY_3] = {.name = "3333"},
};


// GET & SET ===========================================================================================================
static Intel* INTEL_GetIntelSet(const int len) {
    REQ_CONDITION(len > 0, return NULL);

    Intel* intelSet = calloc(len, sizeof(Intel));
    REQ_CONDITION(intelSet != NULL, return NULL);

    return intelSet;
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


// CREATE & DELETE =====================================================================================================
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


// INIT & EXIT =========================================================================================================
bool INTEL_Init() {
    testIntelNet = INTEL_CreateIntelNet();
    INTEL_AppendIntelNet(testIntelNet, (Intel){INTEL_STATE_SRC_TRUE, 0, 1, 1});
    INTEL_AppendIntelNet(testIntelNet, (Intel){INTEL_STATE_SRC_TRUE, 0, 1, 2});
    INTEL_AppendIntelNet(testIntelNet, (Intel){INTEL_STATE_SRC_TRUE, 3, 1, 2});
    for (int i = 0; i < testIntelNet->len; i++) {
        printf("%s: %d, %d, %d\n", __func__,
            testIntelNet->intelSet[i].subject,
            testIntelNet->intelSet[i].action,
            testIntelNet->intelSet[i].object
            );
    }

    return true;
}
void INTEL_Exit() {
    testIntelNet = INTEL_DeleteIntelNet(testIntelNet);
}


// RENEW ===============================================================================================================
SDL_FPoint repulsion[NUM_ENTITIES] = {0};
SDL_FPoint gravitation[NUM_ENTITIES] = {0};
SDL_FPoint gravity[NUM_ENTITIES] = {0};


bool INTEL_Renew() {
    for (int i = 0; i < NUM_ENTITIES; i++) entitySet[i].visible = false;
    for (int i = 0; i < testIntelNet->len; i++) {
        entitySet[testIntelNet->intelSet[i].subject].visible = true;
        entitySet[testIntelNet->intelSet[i].object].visible = true;
    }

    // 斥力
    // SDL_FPoint repulsion[NUM_ENTITIES] = {0};
    for (int i = 0; i < NUM_ENTITIES; i++) {
        if (entitySet[i].visible == false) continue;
        const SDL_FPoint A = entitySet[i].position;

        for (int j = 0; j < NUM_ENTITIES; j++) {
            if (entitySet[j].visible == false) continue;
            const SDL_FPoint B = entitySet[j].position;

            SDL_FPoint AB = {B.x - A.x, B.y - A.y};
            if (AB.x == 0 && AB.y == 0) AB = (SDL_FPoint){SDL_randf() - 0.5f, SDL_randf() - 0.5f};

            const float normAB = SDL_sqrtf(AB.x * AB.x + AB.y * AB.y);
            const float force = -10 / (normAB * normAB);
            repulsion[i] = (SDL_FPoint){force * AB.x / normAB, force * AB.y / normAB};
        }
    }

    // 引力
    // SDL_FPoint gravitation[NUM_ENTITIES] = {0};
    for (int k = 0; k < testIntelNet->len; k++) {
        const Intel intel = testIntelNet->intelSet[k];
        if (intel.state == INTEL_STATE_NULL) continue;

        const int i = intel.subject, j = intel.object;
        const SDL_FPoint A = entitySet[i].position;
        const SDL_FPoint B = entitySet[j].position;

        const SDL_FPoint AB = {B.x - A.x, B.y - A.y};
        const float normAB = SDL_sqrtf(AB.x * AB.x + AB.y * AB.y);
        const float force = AB.x == 0 && AB.y == 0 ? 0 : normAB;
        gravitation[i] = (SDL_FPoint){force * AB.x / normAB, force * AB.y / normAB};
    }

    // 重力
    // SDL_FPoint gravity[NUM_ENTITIES] = {0};
    for (int i = 0; i < NUM_ENTITIES; i++) {
        if (entitySet[i].visible == false) continue;
        const SDL_FPoint A = entitySet[i].position;
        const SDL_FPoint B = {windowRect.w / 2, windowRect.h / 2};
        const SDL_FPoint AB = {B.x - A.x, B.y - A.y};
        gravity[i] = AB;
    }

    // 位移
    for (int i = 0; i < NUM_ENTITIES; i++) {
        const SDL_FPoint points[] = {repulsion[i], gravity[i]};
        const SDL_FPoint dv = SDL_GetSumFPoint(len_of(points), points);
        entitySet[i].position.x += 0.001f * dv.x;
        entitySet[i].position.y += 0.001f * dv.y;
    }

    return true;
}


// DRAW ================================================================================================================
bool INTEL_Draw() {
    for (int i = 0; i < NUM_ENTITIES; i++) {
        if (entitySet[i].visible == false) continue;
        const SDL_FPoint A = entitySet[i].position;

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_FRect rect = {A.x - 20, A.y - 20, 40, 40};
        SDL_RenderRect(renderer, &rect);

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderLine(renderer, A.x, A.y, A.x + repulsion[i].x, A.y + repulsion[i].y);


        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderLine(renderer, A.x, A.y, A.x + gravity[i].x, A.y + gravity[i].y);
    }
    return true;
}
