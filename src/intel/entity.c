#include "entity.h"


Entity entitySet[NUM_ENTITIES] = {
    [ENTITY_UNKNOWN] = {.name = "unknown"},
    [ENTITY_SOCRATES] = {.name = "Socrates"},
    [ENTITY_HUMAN] = {.name = "Human"},
    [ENTITY_DEATH] = {.name = "Death"},
};
static const SDL_Color BACK_COLOR = {64, 64, 64, 128};
static const SDL_Color EDGE_COLOR = {255, 255, 255, 255};
static const SDL_Color FONT_COLOR = {255, 255, 255, 255};
static const char* FONT_PATH = "../res/font/Courier New.ttf";
static const float FONT_SIZE = 48;
static const float MOVE_SPEED = 0.001f;


// INIT & EXIT =========================================================================================================
bool INTEL_InitEntity() {
    TTF_Font* font = TTF_OpenFont(FONT_PATH, FONT_SIZE);
    REQ_CONDITION(font != NULL, return false);
    for (int i = 0; i < NUM_ENTITIES; i++) {
        const char* string = entitySet[i].name == NULL ? "????" : entitySet[i].name;
        entitySet[i].tex = TXT_LoadTexture(renderer, font, string, FONT_COLOR);
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


// RENEW ===============================================================================================================
static void INTEL_RenewEntity_Repulsion() {
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
    }
}
static void INTEL_RenewEntity_Gravitation() {
    for (int k = 0; k < intelNetNow->len; k++) {
        const Intel intel = intelNetNow->intelSet[k];
        if (intel.state == INTEL_STATE_NULL) continue;

        const int i = intel.subject, j = intel.object;
        const SDL_FPoint A = entitySet[i].position;
        const SDL_FPoint B = entitySet[j].position;

        const SDL_FPoint AB = {B.x - A.x, B.y - A.y};
        entitySet[i].gravitation = AB;
        entitySet[j].gravitation = (SDL_FPoint){-AB.x, -AB.y};
    }
}
static void INTEL_RenewEntity_Gravity() {
    for (int i = 0; i < NUM_ENTITIES; i++) {
        if (entitySet[i].visible == false) continue;
        const SDL_FPoint A = entitySet[i].position;
        const SDL_FPoint B = {0, 0};
        const SDL_FPoint AB = {B.x - A.x, B.y - A.y};
        entitySet[i].gravity = AB;
    }
}
static void INTEL_RenewEntity_Position() {
    for (int i = 0; i < NUM_ENTITIES; i++) {
        const SDL_FPoint points[] = {
            entitySet[i].repulsion,
            // entitySet[i].gravitation,
            entitySet[i].gravity,
        };
        const SDL_FPoint dv = SDL_GetSumFPoint(len_of(points), points);
        entitySet[i].position.x += MOVE_SPEED * dv.x;
        entitySet[i].position.y += MOVE_SPEED * dv.y;
    }
}
bool INTEL_RenewEntity() {
    for (int i = 0; i < NUM_ENTITIES; i++) entitySet[i].visible = false;
    for (int i = 0; i < intelNetNow->len; i++) {
        if (intelNetNow->intelSet[i].state == INTEL_STATE_NULL) continue;
        entitySet[intelNetNow->intelSet[i].subject].visible = true;
        entitySet[intelNetNow->intelSet[i].object].visible = true;
    }
    INTEL_RenewEntity_Repulsion();
    INTEL_RenewEntity_Gravitation();
    INTEL_RenewEntity_Gravity();
    INTEL_RenewEntity_Position();
    return true;
}


// DRAW ================================================================================================================
bool INTEL_DrawEntity() {
    for (int i = 0; i < NUM_ENTITIES; i++) {
        if (entitySet[i].visible == false) continue;
        const SDL_FPoint A = INTEL_GetScaledPos(entitySet[i].position);
        const float w = (float)entitySet[i].tex->w, h = (float)entitySet[i].tex->h;
        SDL_FRect rect = {A.x - w / 2, A.y - h / 2, w, h};
        SDL_SetRenderColor(renderer, BACK_COLOR);
        SDL_RenderFillRect(renderer, &rect);
        SDL_SetRenderColor(renderer, EDGE_COLOR);
        SDL_RenderRect(renderer, &rect);
        SDL_RenderTexture(renderer, entitySet[i].tex, NULL, &rect);

        if (DEBUG_GetShift()) {
            const SDL_FPoint R = INTEL_GetScaledPos((SDL_FPoint){
                entitySet[i].position.x + entitySet[i].repulsion.x,
                entitySet[i].position.y + entitySet[i].repulsion.y
            });
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderLine(renderer, A.x, A.y, R.x, R.y);

            const SDL_FPoint G = INTEL_GetScaledPos((SDL_FPoint){
                entitySet[i].position.x + entitySet[i].gravitation.x,
                entitySet[i].position.y + entitySet[i].gravitation.y
            });
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            SDL_RenderLine(renderer, A.x, A.y, G.x, G.y);

            const SDL_FPoint B = INTEL_GetScaledPos((SDL_FPoint){
                entitySet[i].position.x + entitySet[i].gravity.x,
                entitySet[i].position.y + entitySet[i].gravity.y
            });
            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
            SDL_RenderLine(renderer, A.x, A.y, B.x, B.y);
        }
    }
    return true;
}

