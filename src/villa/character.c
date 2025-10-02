#include "character.h"



const float WALK_TIME_PER_CELL = 0.3f;


SDL_FRect direct2rect2[VILLA_NUM_DIRECTS] = {
    [VILLA_DIRECT_PX] = {0, 0, 0.25f, 0.25f},
    [VILLA_DIRECT_NX] = {0, 0.25f, 0.25f, 0.25f},
    [VILLA_DIRECT_PY] = {0, 0.5f, 0.25f, 0.25f},
    [VILLA_DIRECT_NY] = {0, 0.75f, 0.25f, 0.25f},
};
struct Character {
    Model* model;
    Coord coord1, coord2;
    float t1, t2;
};


// SET & GET ===========================================================================================================
bool VILLA_GetIfSomeoneThere(const Coord coord) {
    for (int i = 0; i < characterTable.len; i++) {
        const Character* character = characterTable.kv[i].val;
        if (character->coord2.room == coord.room &&
            character->coord2.x == coord.x &&
            character->coord2.y == coord.y) {
            return true;
        }
    }
    return false;
}
bool VILLA_SetCharacterCoord(Character* character, const Coord coord) {
    REQ_CONDITION(character != NULL, return false);
    if (VILLA_GetRoomCellEmpty(coord) == false) return false;
    character->coord1 = coord;
    character->coord2 = coord;
    return true;
}
bool VILLA_SetCharacterMove(Character* character, int direct) {
    REQ_CONDITION(character != NULL, return false);

    const float time = (float)SDL_GetTicks() / 1000;
    if (time < character->t2) return false;

    direct = (direct % 4 + 4) % 4;
    character->coord2.direct = direct;
    Coord coord = character->coord2;
    switch (direct) {
        case VILLA_DIRECT_PX: coord.x++; break;
        case VILLA_DIRECT_PY: coord.y++; break;
        case VILLA_DIRECT_NX: coord.x--; break;
        case VILLA_DIRECT_NY: coord.y--; break;
        default: return false;
    }

    if (VILLA_GetRoomCellEmpty(coord) == false || VILLA_GetIfSomeoneThere(coord) == true) return false;

    character->coord1 = character->coord2;
    character->t1 = time;
    character->coord2 = coord;
    character->t2 = time + WALK_TIME_PER_CELL;

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
    VILLA_Action action = VILLA_ACT_NONE;
    const int direct = (character->coord2.direct - cameraDirect + 4) % 4;
    const float time = (float)SDL_GetTicks() / 1000;
    if (time < character->t2) {
        switch (SDL_GetTicks() / 100 % 4) {
            case 1: action = VILLA_ACT_WALK_1; break;
            case 3: action = VILLA_ACT_WALK_2; break;
            default: break;
        }
    }

    return LOTRI_SetModelSrc(character->model, &TEX_SRC[direct][action]);
}
bool VILLA_RenewCharacter(void *character_void) {
    Character* character = character_void;
    REQ_CONDITION(character != NULL, return false);

    VILLA_RenewCharacter_Src(character);

    const float time = (float)SDL_GetTicks() / 1000;
    if (character->coord2.room != NULL) {
        Vec3f position1, position2;
        REQ_CONDITION(VILLA_GetRoomCellPosition(character->coord1, &position1), return false);
        REQ_CONDITION(VILLA_GetRoomCellPosition(character->coord2, &position2), return false);
        const float rate = (time - character->t1) / (character->t2 - character->t1);
        const Vec3f position = LOTRI_AtvVec(position1, position2, rate, BASIC_AtvLinear);
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


// ?
bool VILLA_Ask(Character* subject, Character* object) {
    static const char* text = "Do you want to be truly alive?";
    static float t1 = 0, t2 = 0;

    const float t = (float)SDL_GetTicks() / 1000;
    const int len = SDL_min(strlen(text), (int)(20 * (t - t1)));
    char string[len+1];
    for (int i = 0; i < len; i++) {
        string[i] = text[i];
    }
    string[len] = '\0';

    SDL_Texture* texture = TXT_LoadTexture(renderer, VILLA_Font, string, (SDL_Color){255, 255, 255, 255});

    SDL_FRect dst;
    const SDL_Color color = {0, 0, 0, 128};
    const SDL_FRect gid = {0, -160, 1, 1};
    SDL_LoadDstRectAligned(&dst, texture, NULL, &gid, NULL, 18);
    SDL_SetRenderColor(renderer, color);
    SDL_RenderFillRect(renderer, &dst);
    SDL_RenderTexture(renderer, texture, NULL, &dst);

    SDL_DestroyTexture(texture);
    return true;
}
