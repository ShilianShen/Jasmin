#include "character.h"



typedef enum VILLA_Direct {
    DIRECT_W,
    DIRECT_A,
    DIRECT_S,
    DIRECT_D,
    NUM_DIRECTS,
} VILLA_Direct;
SDL_FRect direct2rect2[NUM_DIRECTS] = {
    [DIRECT_W] = {0, 0, 0.25f, 0.25f},
    [DIRECT_S] = {0, 0.25f, 0.25f, 0.25f},
    [DIRECT_A] = {0, 0.5f, 0.25f, 0.25f},
    [DIRECT_D] = {0, 0.75f, 0.25f, 0.25f},
};


struct Character {
    Model* model;
    const Room* room;
    int x, y;
};


// SET & GET ===========================================================================================================
bool VILLA_SetCharacterPosition(Character* character, const Room* room, const int x, const int y) {
    if (character == NULL || VILLA_GetRoomCellEmpty(room, x, y) == false) return false;

    character->room = room;
    character->x = x;
    character->y = y;
    return true;
}


// CREATE & DELETE =====================================================================================================
static bool VILLA_CreateCharacter_RK(Character* character, const cJSON* character_json) {
    memset(character, 0, sizeof(*character));
    if (cJSON_IsObject(character_json) == false) {
        printf("cJSON_IsObject() failed\n");
        return false;
    }

    char* key = NULL;
    char* model_json = NULL;
    char* material_json = NULL;

    if (cJSON_LoadFromObj(character_json, key = "model", JSM_STRING, &model_json) == false) {
        printf("%s: failed in %s\n", __func__, key);
        return false;
    }
    if (cJSON_LoadFromObj(character_json, key = "material", JSM_STRING, &material_json) == false) {
        printf("%s: failed in %s\n", __func__, key);
        return false;
    }

    character->model = LOTRI_CreateModel(model_json, material_json, MODEL_SIDE_CAMERA);
    if (character->model == NULL) {
        printf("%s: character->model == NULL\n", __func__);
        return false;
    }

	return true;
}

void *VILLA_CreateCharacter(const cJSON *character_json) {
    if (character_json == NULL) {
        printf("%s: character_json == NULL.\n", __func__);
        return NULL;
    }
    Character* character = malloc(sizeof(Character));
    if (character == NULL) {
    	printf("%s: character == NULL.\n", __func__);
        return character;
    }
    if (VILLA_CreateCharacter_RK(character, character_json) == false) {
    	printf("%s: VILLA_CreateCharacter_RK failed.\n", __func__);
        VILLA_DestroyCharacter(character);
    }
    return character;
}
void VILLA_DestroyCharacter_V(void* character_void) {
    Character* character = character_void;
    if (character != NULL) {
        LOTRI_DestroyModel(character->model);
        character->model = NULL;
        free(character);
    }
}
void VILLA_DestroyCharacter(Character *character) {
  	if (character != NULL) {
  	    LOTRI_DestroyModel(character->model);
  	    character->model = NULL;
        free(character);
  	}
}


// RENEW ===============================================================================================================
static bool VILLA_RenewCharacter_Src(const Character* character) {
    float a = 0;
    LOTRI_GetModelCZ(character->model, &a);
    a = loop(0, a, M_PI * 2);
    if (7 * M_PI_4 < a || a <= 1 * M_PI_4) return LOTRI_SetModelSrc(character->model, &direct2rect2[DIRECT_W]);
    if (1 * M_PI_4 < a && a <= 3 * M_PI_4) return LOTRI_SetModelSrc(character->model, &direct2rect2[DIRECT_A]);
    if (3 * M_PI_4 < a && a <= 5 * M_PI_4) return LOTRI_SetModelSrc(character->model, &direct2rect2[DIRECT_S]);
    if (5 * M_PI_4 < a && a <= 7 * M_PI_4) return LOTRI_SetModelSrc(character->model, &direct2rect2[DIRECT_D]);
    return false;
}
bool VILLA_RenewCharacter(void *character_void) {
    Character* character = character_void;
    if (character == NULL) {
        printf("%s: character == NULL.\n", __func__);
        return false;
    }
    VILLA_RenewCharacter_Src(character);
    LOTRI_RenewModel(character->model);
    return true;
}


// DRAW ================================================================================================================
bool VILLA_DrawCharacter(const void *character_void) {
    const Character* character = (Character*)character_void;
    if (character == NULL) {
        printf("%s: character == NULL.\n", __func__);
        return false;
    }

    LOTRI_DrawModel(character->model);
    return true;
}
