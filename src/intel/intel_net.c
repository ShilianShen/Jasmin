#include "intel_net.h"
#include "entity.h"
#include "action.h"
#include "intel_arr.h"


static SDL_Texture *entityTex[NUM_ENTITIES], *actionTex[NUM_ACTIONS];
static TTF_Font *entityFont = NULL, *actionFont = NULL;
static const SDL_Color BACK_COLOR = {32, 32, 32, 192}, TEXT_COLOR = {255, 255, 255, 255};
static struct {bool visible; SDL_FRect rect; SDL_FPoint repulsion, gravitation, gravity, position;} entityInfo[NUM_ENTITIES];
static const float MOVE_SPEED = 1.f;
const SDL_FPoint scale = {500, 300};


// GET & SET ===========================================================================================================
void INTEL_ResetIntelNet() {
    for (int i = 0; i < NUM_ENTITIES; i++) {
        entityInfo[i].position = (SDL_FPoint){2 * SDL_randf() - 1, 2 * SDL_randf() - 1};
    }
}
SDL_FPoint INTEL_GetScaledPos(const SDL_FPoint pos) {
    const SDL_FPoint scaledPos = {
        windowRect.x + windowRect.w / 2 + scale.x * pos.x,
        windowRect.y + windowRect.h / 2 + scale.y * pos.y
    };
    return scaledPos;
}
SDL_FPoint INTEL_GetDescalePos(const SDL_FPoint pos) {
    const SDL_FPoint descalePos = {
        (pos.x - windowRect.x - windowRect.w / 2) / scale.x,
        (pos.y - windowRect.y - windowRect.h / 2) / scale.y,
    };
    return descalePos;
}


// TRIG ================================================================================================================
EntityId entityMoveId = 0;
void TRIG_MoveEntity(const void* para) {
    if (entityMoveId == 0) return;
    entityInfo[entityMoveId].position = INTEL_GetDescalePos(PERPH_GetMousePos());
}
Trig trigMove = {TRIG_MoveEntity, NULL, true};


// INIT & EXIT =========================================================================================================
bool INTEL_InitIntelNet() {
    entityFont = TTF_OpenFont(ENTITY_FONT);
    REQ_CONDITION(entityFont != NULL, return false);

    actionFont = TTF_OpenFont(ACTION_FONT);
    REQ_CONDITION(actionFont != NULL, return false);

    for (int i = 0; i < NUM_ENTITIES; i++) {
        entityTex[i] = TXT_LoadTexture(renderer, entityFont, ENTITY_NAMES[i], WHITE);
        REQ_CONDITION(entityTex[i] != NULL, return false);
    }

    for (int i = 0; i < NUM_ACTIONS; i++) {
        actionTex[i] = TXT_LoadTexture(renderer, actionFont, ACTION_NAMES[i], WHITE);
        REQ_CONDITION(actionTex[i] != NULL, return false);
    }

    return true;
}
void INTEL_ExitIntelNet() {
    for (int i = 0; i < NUM_ENTITIES; i++) {
        SDL_DestroyTexture(entityTex[i]);
        entityTex[i] = NULL;
    }
    for (int i = 0; i < NUM_ACTIONS; i++) {
        SDL_DestroyTexture(actionTex[i]);
        actionTex[i] = NULL;
    }
    TTF_CloseFont(entityFont); entityFont = NULL;
    TTF_CloseFont(actionFont); actionFont = NULL;
}


// RENEW ===============================================================================================================
static void INTEL_RenewIntelNet_EntityInfoVisible(IntelArr* intelArr) {
    for (int i = 0; i < NUM_ENTITIES; i++) {
        entityInfo[i].visible = false;
        entityInfo[i].repulsion = entityInfo[i].gravitation = entityInfo[i].gravity = (SDL_FPoint){0};
    }
    for (int k = 0; k < intelArr->len; k++) {
        if (intelArr->arr[k].effective == false) continue;
        entityInfo[intelArr->arr[k].subject].visible = true;
        entityInfo[intelArr->arr[k].object].visible = true;
    }
}
static void INTEL_RenewIntelNet_EntityInfoRepulsion() {
    for (int i = 0; i < NUM_ENTITIES; i++) {
        if (entityInfo[i].visible == false) continue;
        for (int j = 0; j < NUM_ENTITIES; j++) {
            if (entityInfo[j].visible == false || i == j) continue;
            const SDL_FPoint A = entityInfo[i].position;
            const SDL_FPoint B = entityInfo[j].position;

            SDL_FPoint AB = {B.x - A.x, B.y - A.y};
            if (AB.x == 0 && AB.y == 0) AB = (SDL_FPoint){SDL_randf() - 0.5f, SDL_randf() - 0.5f};

            const float normAB = SDL_sqrtf(AB.x * AB.x + AB.y * AB.y);
            const float force = -1 / (normAB * normAB);

            entityInfo[i].repulsion.x += force * AB.x / normAB;
            entityInfo[i].repulsion.y += force * AB.y / normAB;
        }
    }
}
static void INTEL_RenewIntelNet_EntityInfoGravitation(IntelArr* intelArr) {
    for (int k = 0; k < intelArr->len; k++) {
        const Intel intel = intelArr->arr[k];
        if (intel.effective == false) continue;

        const int i = intel.subject, j = intel.object;
        const SDL_FPoint A = entityInfo[i].position;
        const SDL_FPoint B = entityInfo[j].position;

        const SDL_FPoint AB = {B.x - A.x, B.y - A.y};
        entityInfo[i].gravitation.x += AB.x;
        entityInfo[i].gravitation.y += AB.y;
        entityInfo[j].gravitation.x -= AB.x;
        entityInfo[j].gravitation.y -= AB.y;
    }
}
static void INTEL_RenewIntelNet_EntityInfoGravity() {
    for (int i = 0; i < NUM_ENTITIES; i++) {
        if (entityInfo[i].visible == false) continue;
        const SDL_FPoint A = entityInfo[i].position;
        const SDL_FPoint B = {0, 0};
        const SDL_FPoint AB = {B.x - A.x, B.y - A.y};
        entityInfo[i].gravity = AB;
    }
}
static void INTEL_RenewIntelNet_EntityInfoPosition() {
    static float t1 = 0;
    const float t2 = (float)SDL_GetTicks() / 1000;
    const float dt = t2 - t1;
    for (int i = 0; i < NUM_ENTITIES; i++) {
        if (i == entityMoveId) continue;
        const SDL_FPoint points[] = {
            entityInfo[i].repulsion,
            entityInfo[i].gravitation,
            entityInfo[i].gravity,
        };
        const SDL_FPoint dv = SDL_GetSumFPoint(len_of(points), points);
        entityInfo[i].position.x += MOVE_SPEED * dv.x * dt;
        entityInfo[i].position.y += MOVE_SPEED * dv.y * dt;
    }
    t1 = t2;
}
static void INTEL_RenewIntelNet_EntityInfoRectTrig() {
    for (int i = 0; i < NUM_ENTITIES; i++) {
        if (entityInfo[i].visible == false) continue;

        const SDL_FPoint position = INTEL_GetScaledPos(entityInfo[i].position);
        entityInfo[i].rect.w = (float)entityTex[i]->w;
        entityInfo[i].rect.h = (float)entityTex[i]->h;
        entityInfo[i].rect.x = position.x - entityInfo[i].rect.w / 2;
        entityInfo[i].rect.y = position.y - entityInfo[i].rect.h / 2;

        if (PERPH_GetMouseLeftPressed() && PERPH_GetMouseInRect(entityInfo[i].rect)) {
            entityMoveId = i;
            PERPH_SetMouseLeftTrig(&trigMove);
        }
        if (PERPH_GetMouseLeftPressed() == false) entityMoveId = 0;
    }
}
static bool INTEL_RenewIntelNet_EntityInfo(IntelArr* intelArr) {
    INTEL_RenewIntelNet_EntityInfoVisible(intelArr);
    INTEL_RenewIntelNet_EntityInfoRepulsion();
    INTEL_RenewIntelNet_EntityInfoGravitation(intelArr);
    INTEL_RenewIntelNet_EntityInfoGravity();
    INTEL_RenewIntelNet_EntityInfoPosition();
    INTEL_RenewIntelNet_EntityInfoRectTrig();
    return true;
}
bool INTEL_RenewIntelNet(IntelArr* intelArr) {
    for (int k = 0; k < intelArr->len; k++) {
        const Intel intel = intelArr->arr[k];
        if (intel.subject == ENTITY_NULL && intel.action == ACTION_NULL && intel.object == ENTITY_NULL) {
            // intelArrNow->arr[k].effective == false;
        }
        if (intel.judge == JUDGE_AUTO) {
            intelArr->arr[k].state = INTEL_GetAutoState(intel);
        }
        const int i = intel.subject, j = intel.object;
        const SDL_FPoint A = INTEL_GetScaledPos(entityInfo[i].position);
        const SDL_FPoint B = INTEL_GetScaledPos(entityInfo[j].position);
        const SDL_FPoint M = {(A.x + B.x) / 2, (A.y + B.y) / 2};
        const float w = (float)actionTex[intel.action]->w;
        const float h = (float)actionTex[intel.action]->h;
        intelArr->arr[k].rect = (SDL_FRect){M.x - w / 2, M.y - h / 2, w, h};
    }
    INTEL_RenewIntelNet_EntityInfo(intelArr);
    return true;
}


// DRAW ================================================================================================================
bool INTEL_DrawIntelNet(IntelArr* intelArr) {
    const float time = (float)SDL_GetTicks();
    for (int k = 0; k < intelArr->len; k++) {
        const Intel intel = intelArr->arr[k];
        if (intel.effective == false) continue;

        const int i = intel.subject, j = intel.object;
        const SDL_FPoint A = INTEL_GetScaledPos(entityInfo[i].position);
        const SDL_FPoint B = INTEL_GetScaledPos(entityInfo[j].position);
        const SDL_FRect rect = intelArr->arr[k].rect;

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        if (actionSet[intel.action].type == ACTION_TYPE_ONE_WAY) {
            SDL_RenderDashedLine(renderer, A, B, 5, 5, time / 10);
        }
        else SDL_RenderLine(renderer, A.x, A.y, B.x, B.y);

        if (PERPH_GetMouseLeftInRect(rect)) {
            DEBUG_SendMessageR("%s: %s.\n", __func__, INTEL_GetStrIntel(intel));
        }

        SDL_Color text = JUDGE_COLORS[intel.judge], back = STATE_COLORS[intel.state];
        if (PERPH_GetMouseLeftInRect(rect)) {
            const SDL_Color temp = text;
            text = back;
            back = temp;
        }

        SDL_SetRenderColor(renderer, back);
        SDL_RenderFillRect(renderer, &rect);

        SDL_SetTextureColorRGB(actionTex[intel.action], text);
        SDL_RenderTexture(renderer, actionTex[intel.action], NULL, &rect);
    }

    for (int i = 0; i < NUM_ENTITIES; i++) {
        if (entityInfo[i].visible == false) continue;

        SDL_Color back = BACK_COLOR, text = TEXT_COLOR;
        if (i == entityMoveId) {
            const SDL_Color temp = text;
            text = back;
            back = temp;
        }

        SDL_SetRenderColor(renderer, back);
        SDL_RenderFillRect(renderer, &entityInfo[i].rect);

        SDL_SetRenderColor(renderer, text);
        SDL_RenderRect(renderer, &entityInfo[i].rect);

        SDL_SetTextureColorRGB(entityTex[i], text);
        SDL_RenderTexture(renderer, entityTex[i], NULL, &entityInfo[i].rect);

        const SDL_FPoint A = INTEL_GetScaledPos(entityInfo[i].position);
        if (DEBUG_GetShift()) {
            const SDL_FPoint R = INTEL_GetScaledPos((SDL_FPoint){
                entityInfo[i].position.x + entityInfo[i].repulsion.x,
                entityInfo[i].position.y + entityInfo[i].repulsion.y
            });
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderLine(renderer, A.x, A.y, R.x, R.y);

            const SDL_FPoint G = INTEL_GetScaledPos((SDL_FPoint){
                entityInfo[i].position.x + entityInfo[i].gravitation.x,
                entityInfo[i].position.y + entityInfo[i].gravitation.y
            });
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            SDL_RenderLine(renderer, A.x, A.y, G.x, G.y);

            const SDL_FPoint B = INTEL_GetScaledPos((SDL_FPoint){
                entityInfo[i].position.x + entityInfo[i].gravity.x,
                entityInfo[i].position.y + entityInfo[i].gravity.y
            });
            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
            SDL_RenderLine(renderer, A.x, A.y, B.x, B.y);
        }
    }
    return true;
}

