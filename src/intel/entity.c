#include "entity.h"



Entity entitySet[NUM_ENTITIES] = {
    [ENTITY_UNKNOWN] = {.name = "unknown"},
    [ENTITY_SOCRATES] = {.name = "Socrates"},
    [ENTITY_HUMAN] = {.name = "Human"},
    [ENTITY_DEATH] = {.name = "Death"},
};


bool INTEL_InitEntity() {
    TTF_Font* font = TTF_OpenFont("../res/font/Courier New.ttf", 48);
    for (int i = 0; i < NUM_ENTITIES; i++) {
        const char* string = entitySet[i].name == NULL ? "????" : entitySet[i].name;
        entitySet[i].tex = TXT_LoadTexture(renderer, font, string, (SDL_Color){255, 255, 255, 255});
        REQ_CONDITION(entitySet[i].tex != NULL, {
            TTF_CloseFont(font); font = NULL;
            return false;
        });
    }
    TTF_CloseFont(font); font = NULL;
    return true;
}
void INTEL_ExitEntity() {
    for (int i = 0; i < NUM_ENTITIES; i++) {
        SDL_DestroyTexture(entitySet[i].tex);
        entitySet[i].tex = NULL;
    }
}


bool INTEL_RenewEntity(const IntelNet* intelNet) {
    for (int i = 0; i < NUM_ENTITIES; i++) entitySet[i].visible = false;
    for (int i = 0; i < intelNet->len; i++) {
        if (intelNet->intelSet[i].state == INTEL_STATE_NULL) continue;
        entitySet[intelNet->intelSet[i].subject].visible = true;
        entitySet[intelNet->intelSet[i].object].visible = true;
    }

    for (int i = 0; i < NUM_ENTITIES; i++) {
        if (entitySet[i].visible == false) continue;
        entitySet[i].repulsion = (SDL_FPoint){0, 0};
        for (int j = 0; j < NUM_ENTITIES; j++) {
            if (entitySet[j].visible == false || i == j) continue;
            const SDL_FPoint A = entitySet[i].position;
            const SDL_FPoint B = entitySet[j].position;

            SDL_FPoint AB = {B.x - A.x, B.y - A.y};
            if (AB.x == 0 && AB.y == 0) AB = (SDL_FPoint){SDL_randf() - 0.5f, SDL_randf() - 0.5f};

            const float normAB = SDL_sqrtf(AB.x * AB.x + AB.y * AB.y);
            const float force = -1 / (normAB * normAB);

            entitySet[i].repulsion.x += force * AB.x / normAB;
            entitySet[i].repulsion.y += force * AB.y / normAB;
        }
    } // 斥力
    for (int k = 0; k < intelNet->len; k++) {
        const Intel intel = intelNet->intelSet[k];
        if (intel.state == INTEL_STATE_NULL) continue;

        const int i = intel.subject, j = intel.object;
        const SDL_FPoint A = entitySet[i].position;
        const SDL_FPoint B = entitySet[j].position;

        const SDL_FPoint AB = {B.x - A.x, B.y - A.y};
        entitySet[i].gravitation = AB;
        entitySet[j].gravitation = (SDL_FPoint){-AB.x, -AB.y};
    } // 引力
    for (int i = 0; i < NUM_ENTITIES; i++) {
        if (entitySet[i].visible == false) continue;
        const SDL_FPoint A = entitySet[i].position;
        const SDL_FPoint B = {0, 0};
        const SDL_FPoint AB = {B.x - A.x, B.y - A.y};
        entitySet[i].gravity = AB;
    } // 重力
    for (int i = 0; i < NUM_ENTITIES; i++) {
        const SDL_FPoint points[] = {
            entitySet[i].repulsion,
            // entitySet[i].gravitation,
            entitySet[i].gravity,
        };
        const SDL_FPoint dv = SDL_GetSumFPoint(len_of(points), points);
        const float rate = 0.01f;
        entitySet[i].position.x += rate * dv.x;
        entitySet[i].position.y += rate * dv.y;
    } // 位移
    return true;
}
bool INTEL_DrawEntity() {
    for (int i = 0; i < NUM_ENTITIES; i++) {
        if (entitySet[i].visible == false) continue;
        const SDL_FPoint A = {
            windowRect.w / 2 + scale.x * entitySet[i].position.x,
            windowRect.h / 2 + scale.y * entitySet[i].position.y
        };
        const float w = (float)entitySet[i].tex->w, h = (float)entitySet[i].tex->h;
        SDL_FRect rect = {A.x - w / 2, A.y - h / 2, w, h};
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
        SDL_RenderFillRect(renderer, &rect);
        SDL_RenderTexture(renderer, entitySet[i].tex, NULL, &rect);

        if (DEBUG_GetShift()) {
            const SDL_FPoint R = {A.x + scale.x * entitySet[i].repulsion.x, A.y + scale.y * entitySet[i].repulsion.y};
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderLine(renderer, A.x, A.y, R.x, R.y);

            const SDL_FPoint G = {A.x + scale.x * entitySet[i].gravitation.x, A.y + scale.y * entitySet[i].gravitation.y};
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            SDL_RenderLine(renderer, A.x, A.y, G.x, G.y);

            const SDL_FPoint B = {A.x + scale.x * entitySet[i].gravity.x, A.y + scale.y * entitySet[i].gravity.y};
            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
            SDL_RenderLine(renderer, A.x, A.y, B.x, B.y);
        }
    }
    return true;
}