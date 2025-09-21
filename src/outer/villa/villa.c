#include "villa.h"

#include "room.h"
#include "weather.h"
#include "character.h"


// Character *characterBW = NULL, *characterOT = NULL;
Table characterTable;
Table roomTable;


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



void VILLA_DeleteCharacterTable() {
    for (int i = 0; i < characterTable.len; i++) {
        if (characterTable.kv[i].key != NULL) {
            free(characterTable.kv[i].key);
            characterTable.kv[i].key = NULL;
        }
        if (characterTable.kv[i].val != NULL) {
            VILLA_DestroyCharacter(characterTable.kv[i].val);
            characterTable.kv[i].val = NULL;
        }
    }
}
bool VILLA_Init() {
    cJSON* characterTable_json = getJson("../config/villa_character.json");
    BASIC_CreateTable(&characterTable, characterTable_json, VILLA_CreateCharacter);
    cJSON_Delete(characterTable_json);

    cJSON* roomTable_json = getJson("../config/villa_room.json");
    BASIC_CreateTable(&roomTable, roomTable_json, VILLA_CreateRoom);
    cJSON_Delete(roomTable_json);

    VILLA_InitRain();
    return true;
}
static bool VILLA_Renew_Camera() {
    static DelayVec3f smallRotate = {0}, bigRotate = {0};
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
    for (int i = 0; i < roomTable.len; i++) {
        VILLA_RenewRoom(roomTable.kv[i].val);
    }
    for (int i = 0; i < characterTable.len; i++) {
        VILLA_RenewCharacter(characterTable.kv[i].val);
    }
    return true
    && VILLA_Renew_Camera()
    ;
}
bool VILLA_Draw() {
    for (int i = 0; i < roomTable.len; i++) {
        VILLA_DrawRoom(roomTable.kv[i].val);
    }
    for (int i = 0; i < characterTable.len; i++) {
        VILLA_DrawCharacter(characterTable.kv[i].val);
    }
    return true
    && VILLA_DrawRain()
    ;
}
void VILLA_Exit() {
    BASIC_DeleteTable(&characterTable, VILLA_DestroyCharacter_V);
    BASIC_DeleteTable(&roomTable, VILLA_DestroyRoom_V);
}
