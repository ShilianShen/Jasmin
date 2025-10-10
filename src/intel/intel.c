#include "intel.h"

#include "../debug/debug.h"


const char* INTEL_STATE_STRING[INTEL_NUM_STATES] = {
    [INTEL_STATE_NULL] = "NULL",
    [INTEL_STATE_SRC_FALSE] = "SRC_FALSE",
    [INTEL_STATE_SRC_TRUE] = "SRC_TRUE",
    [INTEL_STATE_UNKNOWN] = "UNKNOWN",
};
IntelNet* testIntelNet = NULL;
TTF_Font* font = NULL;


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
    SDL_Texture* tex;
} Entity;
Entity entitySet[NUM_ENTITIES] = {
    [ENTITY_0] = {.name = "0000"},
    [ENTITY_1] = {.name = "1111"},
    [ENTITY_2] = {.name = "2222"},
    [ENTITY_3] = {.name = "3333"},
};


enum ActionId {
    ACTION_0,
    ACTION_1,
    ACTION_2,
    ACTION_3,
    NUM_ACTIONS,
};
typedef struct {
    const char* name;
    SDL_Texture* tex;
} Action;
Action actionSet[NUM_ACTIONS] = {
    [ACTION_0] = {.name = "ACTION_0"},
    [ACTION_1] = {.name = "ACTION_1"},
    [ACTION_2] = {.name = "ACTION_2"},
    [ACTION_3] = {.name = "ACTION_3"},
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
    font = TTF_OpenFont("../res/font/Courier New.ttf", 48);
    testIntelNet = INTEL_CreateIntelNet();
    INTEL_AppendIntelNet(testIntelNet, (Intel){INTEL_STATE_SRC_TRUE, 0, 1, 1});
    INTEL_AppendIntelNet(testIntelNet, (Intel){INTEL_STATE_SRC_TRUE, 3, 1, 2});

    for (int i = 0; i < NUM_ENTITIES; i++) {
        entitySet[i].tex = TXT_LoadTexture(renderer, font, entitySet[i].name, (SDL_Color){255, 255, 255, 255});
    }
    for (int i = 0; i < NUM_ACTIONS; i++) {
        actionSet[i].tex = TXT_LoadTexture(renderer, font, actionSet[i].name, (SDL_Color){255, 255, 255, 255});
    }

    return true;
}
void INTEL_Exit() {
    for (int i = 0; i < NUM_ENTITIES; i++) {
        SDL_DestroyTexture(entitySet[i].tex); entitySet[i].tex = NULL;
    }
    for (int i = 0; i < NUM_ACTIONS; i++) {
        SDL_DestroyTexture(actionSet[i].tex); actionSet[i].tex = NULL;
    }
    testIntelNet = INTEL_DeleteIntelNet(testIntelNet);
    TTF_CloseFont(font); font = NULL;
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
    for (int i = 0; i < NUM_ENTITIES; i++) {
        if (entitySet[i].visible == false) continue;
        repulsion[i] = (SDL_FPoint){0, 0};
        for (int j = 0; j < NUM_ENTITIES; j++) {
            if (entitySet[j].visible == false || i == j) continue;
            const SDL_FPoint A = entitySet[i].position;
            const SDL_FPoint B = entitySet[j].position;

            SDL_FPoint AB = {B.x - A.x, B.y - A.y};
            if (AB.x == 0 && AB.y == 0) AB = (SDL_FPoint){SDL_randf() - 0.5f, SDL_randf() - 0.5f};

            const float normAB = SDL_sqrtf(AB.x * AB.x + AB.y * AB.y);
            const float force = -1 / (normAB * normAB);

            repulsion[i].x += force * AB.x / normAB;
            repulsion[i].y += force * AB.y / normAB;
        }
    }

    // 引力
    for (int k = 0; k < testIntelNet->len; k++) {
        const Intel intel = testIntelNet->intelSet[k];
        if (intel.state == INTEL_STATE_NULL) continue;

        const int i = intel.subject, j = intel.object;
        const SDL_FPoint A = entitySet[i].position;
        const SDL_FPoint B = entitySet[j].position;

        const SDL_FPoint AB = {B.x - A.x, B.y - A.y};
        gravitation[i] = AB;
        gravitation[j] = (SDL_FPoint){-AB.x, -AB.y};
    }

    // 重力
    for (int i = 0; i < NUM_ENTITIES; i++) {
        if (entitySet[i].visible == false) continue;
        const SDL_FPoint A = entitySet[i].position;
        const SDL_FPoint B = {0, 0};
        const SDL_FPoint AB = {B.x - A.x, B.y - A.y};
        gravity[i] = AB;
    }

    // 位移
    for (int i = 0; i < NUM_ENTITIES; i++) {
        const SDL_FPoint points[] = {
            repulsion[i],
            gravitation[i],
            gravity[i],
        };
        const SDL_FPoint dv = SDL_GetSumFPoint(len_of(points), points);
        const float rate = 0.001f;
        entitySet[i].position.x += rate * dv.x;
        entitySet[i].position.y += rate * dv.y;
    }

    return true;
}


// DRAW ================================================================================================================
bool INTEL_Draw() {
    const SDL_FPoint scale = {500, 300};
    for (int k = 0; k < testIntelNet->len; k++) {
        const Intel intel = testIntelNet->intelSet[k];
        if (intel.state == INTEL_STATE_NULL) continue;

        const int i = intel.subject, j = intel.object;
        const SDL_FPoint A = {
            windowRect.w / 2 + scale.x * entitySet[i].position.x,
            windowRect.h / 2 + scale.y * entitySet[i].position.y
        };
        const SDL_FPoint B = {
            windowRect.w / 2 + scale.x * entitySet[j].position.x,
            windowRect.h / 2 + scale.y * entitySet[j].position.y
        };
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderLine(renderer, A.x, A.y, B.x, B.y);
    }
    for (int i = 0; i < NUM_ENTITIES; i++) {
        if (entitySet[i].visible == false) continue;
        const SDL_FPoint A = {
            windowRect.w / 2 + scale.x * entitySet[i].position.x,
            windowRect.h / 2 + scale.y * entitySet[i].position.y
        };
        const float w = (float)entitySet[i].tex->w, h = (float)entitySet[i].tex->h;
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
        SDL_FRect rect = {A.x - w / 2, A.y - h / 2, w, h};
        SDL_RenderFillRect(renderer, &rect);
        SDL_RenderTexture(renderer, entitySet[i].tex, NULL, &rect);

        if (DEBUG_GetShift()) {
            const SDL_FPoint R = {A.x + scale.x * repulsion[i].x, A.y + scale.y * repulsion[i].y};
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderLine(renderer, A.x, A.y, R.x, R.y);

            const SDL_FPoint G = {A.x + scale.x * gravitation[i].x, A.y + scale.y * gravitation[i].y};
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            SDL_RenderLine(renderer, A.x, A.y, G.x, G.y);

            const SDL_FPoint B = {A.x + scale.x * gravity[i].x, A.y + scale.y * gravity[i].y};
            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
            SDL_RenderLine(renderer, A.x, A.y, B.x, B.y);
        }
    }
    return true;
}
