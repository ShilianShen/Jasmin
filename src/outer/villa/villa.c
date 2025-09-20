#include "villa.h"

#include "room.h"
#include "weather.h"
#include "character.h"


Room* rootRoom = NULL;
Character *characterBW, *characterOT;


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


void VILLA_Renew_Camera() {
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
        static float k = 0;
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
}
bool VILLA_Init() {
    cJSON* room_json = getJson("../res/room/root_room.json");
    cJSON* character_bw_json = getJson("../res/character/test_bw.json");
    cJSON* character_ot_json = getJson("../res/character/test_ot.json");
    {
        rootRoom = VILLA_CreateRoom(room_json);
        characterBW = VILLA_CreateCharacter(character_bw_json);
        characterOT = VILLA_CreateCharacter(character_ot_json);
        if (rootRoom == NULL || characterBW == NULL || characterOT == NULL) {
            return false;
        }
    }
    cJSON_Delete(room_json);
    cJSON_Delete(character_bw_json);
    cJSON_Delete(character_ot_json);

    // LOTRI_SetModelPosition(modelArr[1], (Vec3f){-1, 1, 0});
    VILLA_InitRain();
    return true;
}
bool VILLA_Renew() {
    VILLA_RenewRoom(rootRoom);
    VILLA_RenewCharacter(characterBW);
    VILLA_RenewCharacter(characterOT);

    VILLA_Renew_Camera();
    return true;
}
bool VILLA_Draw() {
    VILLA_DrawRoom(rootRoom);
    VILLA_DrawCharacter(characterBW);
    VILLA_DrawCharacter(characterOT);
    return true
    && VILLA_DrawRain()
    ;
}
void VILLA_Exit() {
    VILLA_DeleteRoom(rootRoom);
    VILLA_DeleteCharacter(characterOT);
    VILLA_DeleteCharacter(characterBW);
}
