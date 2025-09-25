#include "character.h"



const float WALK_TIME_PER_CELL = 0.1f;


SDL_FRect direct2rect2[VILLA_NUM_DIRECTS] = {
    [VILLA_DIRECT_W] = {0, 0, 0.25f, 0.25f},
    [VILLA_DIRECT_S] = {0, 0.25f, 0.25f, 0.25f},
    [VILLA_DIRECT_A] = {0, 0.5f, 0.25f, 0.25f},
    [VILLA_DIRECT_D] = {0, 0.75f, 0.25f, 0.25f},
};
struct Character {
    Model* model;
    Coord coord1, coord2;
    VILLA_Direct direct;
    Delay delay;
};


// SET & GET ===========================================================================================================
VILLA_Direct VILLA_GetRelativeDirect(const VILLA_Direct observedDirect, const VILLA_Direct observerDirect) {
    static const VILLA_Direct RULE[VILLA_NUM_DIRECTS][VILLA_NUM_DIRECTS] = {
        [VILLA_DIRECT_W][VILLA_DIRECT_W] = VILLA_DIRECT_W,
        [VILLA_DIRECT_W][VILLA_DIRECT_A] = VILLA_DIRECT_D,
        [VILLA_DIRECT_W][VILLA_DIRECT_S] = VILLA_DIRECT_S,
        [VILLA_DIRECT_W][VILLA_DIRECT_D] = VILLA_DIRECT_A,

        [VILLA_DIRECT_A][VILLA_DIRECT_W] = VILLA_DIRECT_A,
        [VILLA_DIRECT_A][VILLA_DIRECT_A] = VILLA_DIRECT_W,
        [VILLA_DIRECT_A][VILLA_DIRECT_S] = VILLA_DIRECT_D,
        [VILLA_DIRECT_A][VILLA_DIRECT_D] = VILLA_DIRECT_S,

        [VILLA_DIRECT_S][VILLA_DIRECT_W] = VILLA_DIRECT_S,
        [VILLA_DIRECT_S][VILLA_DIRECT_A] = VILLA_DIRECT_A,
        [VILLA_DIRECT_S][VILLA_DIRECT_S] = VILLA_DIRECT_W,
        [VILLA_DIRECT_S][VILLA_DIRECT_D] = VILLA_DIRECT_D,

        [VILLA_DIRECT_D][VILLA_DIRECT_W] = VILLA_DIRECT_D,
        [VILLA_DIRECT_D][VILLA_DIRECT_A] = VILLA_DIRECT_S,
        [VILLA_DIRECT_D][VILLA_DIRECT_S] = VILLA_DIRECT_A,
        [VILLA_DIRECT_D][VILLA_DIRECT_D] = VILLA_DIRECT_W,
    };
    return RULE[observedDirect][observerDirect];
}
bool VILLA_SetCharacterCoord(Character* character, const Coord coord) {
    REQ_CONDITION(character != NULL, return false);
    character->coord1 = character->coord2;
    if (VILLA_GetRoomCellEmpty(coord) == false) return false;

    character->coord2 = coord;
    return true;
}
bool VILLA_SetCharacterMove(Character* character, const VILLA_Direct direct) {
    REQ_CONDITION(character != NULL, return false);

    Coord coord = character->coord2;
    switch (direct) {
        case VILLA_DIRECT_W: coord.y--; break;
        case VILLA_DIRECT_A: coord.x--; break;
        case VILLA_DIRECT_S: coord.y++; break;
        case VILLA_DIRECT_D: coord.x++; break;
        default: return false;
    }
    character->delay.block = true;
    if (BASIC_SetDelay(&character->delay, WALK_TIME_PER_CELL)) {
        VILLA_SetCharacterCoord(character, coord);
    }
    return true;
}


// CREATE & DELETE =====================================================================================================
static bool VILLA_CreateCharacter_RK(Character* character, const cJSON* character_json) {
    memset(character, 0, sizeof(*character));
    REQ_CONDITION(cJSON_IsObject(character_json), return false);

    char* model_json = NULL;
    REQ_CONDITION(cJSON_Load(character_json, "model", JSM_STRING, &model_json), return false);

    char* material_json = NULL;
    REQ_CONDITION(cJSON_Load(character_json, "material", JSM_STRING, &material_json), return false);

    character->model = LOTRI_CreateModel(model_json, material_json, MODEL_SIDE_CAMERA);
    REQ_CONDITION(character->model != NULL, return false);

	return true;
}
void* VILLA_CreateCharacter(const cJSON *character_json) {
    REQ_CONDITION(character_json != NULL, return NULL);

    Character* character = malloc(sizeof(Character));
    REQ_CONDITION(character != NULL, return NULL);
    REQ_CONDITION(VILLA_CreateCharacter_RK(character, character_json), character = VILLA_DeleteCharacter(character));
    return character;
}
void* VILLA_DeleteCharacter(void *character_void) {
    Character* character = character_void;
  	if (character != NULL) {
  	    LOTRI_DestroyModel(character->model);
  	    character->model = NULL;
        free(character);
  	}
    character = NULL;
    return NULL;
}


// RENEW ===============================================================================================================
static bool VILLA_RenewCharacter_Src(const Character* character) {
    float a = 0;
    LOTRI_GetModelCZ(character->model, &a);
    a = loop(0, a, M_PI * 2);
    VILLA_Action action = VILLA_ACT_NONE;
    const float time = (float)SDL_GetTicks() / 1000;
    if (time < character->delay.t2) {
        switch (SDL_GetTicks() / 70 % 4) {
            case 1: action = VILLA_ACT_WALK_1; break;
            case 3: action = VILLA_ACT_WALK_2; break;
            default: break;
        }
    }

    if (7 * M_PI_4 < a || a <= 1 * M_PI_4)
        return LOTRI_SetModelSrc(character->model, &TEX_SRC[VILLA_DIRECT_W][action]);
    if (1 * M_PI_4 < a && a <= 3 * M_PI_4)
        return LOTRI_SetModelSrc(character->model, &TEX_SRC[VILLA_DIRECT_A][action]);
    if (3 * M_PI_4 < a && a <= 5 * M_PI_4)
        return LOTRI_SetModelSrc(character->model, &TEX_SRC[VILLA_DIRECT_S][action]);
    if (5 * M_PI_4 < a && a <= 7 * M_PI_4)
        return LOTRI_SetModelSrc(character->model, &TEX_SRC[VILLA_DIRECT_D][action]);
    return false;
}
bool VILLA_RenewCharacter(void *character_void) {
    Character* character = character_void;
    REQ_CONDITION(character != NULL, return false);


    DEBUG_SendMessageR("%.2f, %.2f\n", character->delay.t1, character->delay.t2);


    VILLA_RenewCharacter_Src(character);

    if (character->coord2.room != NULL) {
        Vec3f position1, position2;
        REQ_CONDITION(VILLA_GetRoomCellPosition(character->coord1, &position1), return false);
        REQ_CONDITION(VILLA_GetRoomCellPosition(character->coord2, &position2), return false);
        float rate;
        REQ_CONDITION(BASIC_GetDelay(&character->delay, &rate), return false);
        Vec3f position = LOTRI_AtvVec(position1, position2, rate, BASIC_AtvLinear);
        LOTRI_SetModelPosition(character->model, position);
    }
    LOTRI_RenewModel(character->model);
    return true;
}


// DRAW ================================================================================================================
bool VILLA_DrawCharacter(const void *character_void) {
    const Character* character = (Character*)character_void;
    REQ_CONDITION(character != NULL, return false);

    LOTRI_DrawModel(character->model);
    return true;
}
