#include "entity.h"
#include "intel_arr.h"


Entity entitySet[NUM_ENTITIES] = {};
const char* ENTITY_NAMES[NUM_ENTITIES] = {
    [ENTITY_NULL] = "unknown",
    [ENTITY_SOMEONE] = "someone",
    [ENTITY_SOMETHING] = "something",
    [ENTITY_SOCRATES] = "Socrates",
    [ENTITY_HUMAN] = "Human",
    [ENTITY_DEATH] = "Death",
    [ENTITY_FLY] = "Fly",
};
static const SDL_Color BACK_COLOR = {32, 32, 32, 192};
static const SDL_Color TEXT_COLOR = {255, 255, 255, 255};
static const float MOVE_SPEED = 1.f;


// TRIG ================================================================================================================
EntityId entityMoveId = 0;
bool ifTrigSet = false;
void TRIG_MoveEntity(const void* para) {
    if (entityMoveId == 0) return;
    entitySet[entityMoveId].position = INTEL_GetDescalePos(PERPH_GetMousePos());
}
Trig trigMove = {TRIG_MoveEntity, NULL, true};


// GET & SET ===========================================================================================================



// INIT & EXIT =========================================================================================================



// RENEW ===============================================================================================================
static void INTEL_RenewEntity_Repulsion() {
    for (int i = 0; i < NUM_ENTITIES; i++) {
        if (entitySet[i].visible == false) continue;
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
    for (int k = 0; k < intelArrNow->len; k++) {
        const Intel intel = intelArrNow->arr[k];
        if (intel.effective == false) continue;

        const int i = intel.subject, j = intel.object;
        const SDL_FPoint A = entitySet[i].position;
        const SDL_FPoint B = entitySet[j].position;

        const SDL_FPoint AB = {B.x - A.x, B.y - A.y};
        entitySet[i].gravitation.x += AB.x;
        entitySet[i].gravitation.y += AB.y;
        entitySet[j].gravitation.x -= AB.x;
        entitySet[j].gravitation.y -= AB.y;
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
    static float t1 = 0;
    const float t2 = (float)SDL_GetTicks() / 1000;
    const float dt = t2 - t1;
    for (int i = 0; i < NUM_ENTITIES; i++) {
        if (i == entityMoveId) continue;
        const SDL_FPoint points[] = {
            entitySet[i].repulsion,
            entitySet[i].gravitation,
            entitySet[i].gravity,
        };
        const SDL_FPoint dv = SDL_GetSumFPoint(len_of(points), points);
        entitySet[i].position.x += MOVE_SPEED * dv.x * dt;
        entitySet[i].position.y += MOVE_SPEED * dv.y * dt;
    }
    t1 = t2;
}
bool INTEL_RenewEntity() {
    for (int i = 0; i < NUM_ENTITIES; i++) {
        entitySet[i].visible = false;
        entitySet[i].repulsion = entitySet[i].gravitation = entitySet[i].gravity = (SDL_FPoint){0};
    }
    for (int k = 0; k < intelArrNow->len; k++) {
        if (intelArrNow->arr[k].effective == false) continue;
        entitySet[intelArrNow->arr[k].subject].visible = true;
        entitySet[intelArrNow->arr[k].object].visible = true;
    }
    INTEL_RenewEntity_Repulsion();
    INTEL_RenewEntity_Gravitation();
    INTEL_RenewEntity_Gravity();
    INTEL_RenewEntity_Position();

    return true;
}

