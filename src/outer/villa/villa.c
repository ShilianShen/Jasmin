#include "villa.h"

#include "room.h"
#include "weather.h"
#include "character.h"


Room* rootRoom = NULL;
// Character *characterBW = NULL, *characterOT = NULL;
Table characterTable = {0};


typedef enum VILLA_Direct {
    DIRECT_W,
    DIRECT_A,
    DIRECT_S,
    DIRECT_D,
    NUM_DIRECTS,
} VILLA_Direct;
SDL_FRect direct2rect[NUM_DIRECTS] = {
    [DIRECT_W] = {0, 0, 0.25f, 0.25f},
    [DIRECT_S] = {0, 0.25f, 0.25f, 0.25f},
    [DIRECT_A] = {0, 0.5f, 0.25f, 0.25f},
    [DIRECT_D] = {0, 0.75f, 0.25f, 0.25f},
};


bool VILLA_Init() {
    cJSON* room_json = getJson("../res/room/root_room.json");
    cJSON* characterTable_json = getJson("../config/villa_character.json");
    rootRoom = VILLA_CreateRoom(room_json);
    {
        characterTable.len = cJSON_GetArraySize(characterTable_json);
        if (characterTable.len <= 0) {
            printf("%s: characterTable.len <= 0.\n", __func__);
            return false;
        }

        characterTable.kv = calloc(characterTable.len, sizeof(KeyVal));
        if (characterTable.kv == NULL) {
            printf("%s: characterTable.kv == NULL.\n", __func__);
            return false;
        }

        for (int i = 0; i < characterTable.len; i++) {
            const cJSON* character_json = cJSON_GetArrayItem(characterTable_json, i);
            const char* key_json = character_json->string;

            characterTable.kv[i].key = strdup(key_json);
            if (characterTable.kv[i].key == NULL) {
                printf("%s: characterTable.kv[i].key == NULL.\n", __func__);
                return false;
            }

            characterTable.kv[i].val = VILLA_CreateCharacter(character_json);
            if (characterTable.kv[i].val == NULL) {
                printf("%s: characterTable.kv[i].val == NULL.\n", __func__);
                return false;
            }
        }
    }
    if (rootRoom == NULL) {
        return false;
    }
    cJSON_Delete(room_json);
    cJSON_Delete(characterTable_json);

    VILLA_InitRain();
    return true;
}
static bool VILLA_Renew_Camera() {
    static DelayVec3f smallRotate = {0}, bigRotate = {0, 0, 0};
    bigRotate.block = true;
    {
        const float angle = M_PI_4 * 0.5;
        Vec3f v = {0, 0.5f, 0.3f};
        if (DEVICE_GetKeyPressed(SDL_SCANCODE_DOWN )) v.v.y += angle;
        if (DEVICE_GetKeyPressed(SDL_SCANCODE_UP   )) v.v.y -= angle;
        if (DEVICE_GetKeyPressed(SDL_SCANCODE_LEFT )) v.v.z += angle;
        if (DEVICE_GetKeyPressed(SDL_SCANCODE_RIGHT)) v.v.z -= angle;
        LOTRI_SetDelayVec(&smallRotate, v, 1.f);
    }
    {
        static float k = 2;
        Vec3f v = {0, 0, 0};
        if (DEVICE_GetKeyPress(SDL_SCANCODE_Q)) k += 1;
        if (DEVICE_GetKeyPress(SDL_SCANCODE_E)) k -= 1;
        v.v.z += k * (float)M_PI_2;
        LOTRI_SetDelayVec(&bigRotate, v, 0.5f);
    }
    const Vec3f vs = LOTRI_GetDelayVecVec(smallRotate);
    const Vec3f vd = LOTRI_GetDelayVecVec(bigRotate);
    const Vec3f rotate = (Vec3f){
        vs.v.x + vd.v.x,
        vs.v.y + vd.v.y,
        vs.v.z + vd.v.z
    };
    LOTRI_SetCamera(rotate);
    return true;
}
bool VILLA_Renew() {
    VILLA_RenewRoom(rootRoom);
    for (int i = 0; i < characterTable.len; i++) {
        VILLA_RenewCharacter(characterTable.kv[i].val);
    }
    return true
    && VILLA_RenewRoom(rootRoom)
    && VILLA_Renew_Camera()
    ;
}
bool VILLA_Draw() {
    for (int i = 0; i < characterTable.len; i++) {
        VILLA_DrawCharacter(characterTable.kv[i].val);
    }
    return true
    && VILLA_DrawRoom(rootRoom)
    && VILLA_DrawRain()
    ;
}
void VILLA_Exit() {
    VILLA_DeleteRoom(rootRoom);
    for (int i = 0; i < characterTable.len; i++) {
        if (characterTable.kv[i].key != NULL) {
            free(characterTable.kv[i].key);
            characterTable.kv[i].key = NULL;
        }
        if (characterTable.kv[i].val != NULL) {
            VILLA_DeleteCharacter(characterTable.kv[i].val);
            characterTable.kv[i].val = NULL;
        }
    }
}
