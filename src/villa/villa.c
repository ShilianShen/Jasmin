#include "villa.h"

#include "room.h"
#include "weather.h"
#include "character.h"


const char CHARACTER_TABLE_JSON_FILE[] = "../config/villa_character.json";
const char ROOM_TABLE_JSON_FILE[] = "../config/villa_room.json";


Table characterTable;
Table roomTable;
SDL_FRect TEX_SRC[VILLA_NUM_DIRECTS][VILLA_NUM_ACTS];
int cameraDirect;


Character* you = NULL;


bool VILLA_Init() {
    cJSON* characterTable_json = getJson(CHARACTER_TABLE_JSON_FILE);
    BASIC_CreateTable(&characterTable, characterTable_json, VILLA_CreateCharacter);
    cJSON_Delete(characterTable_json);

    cJSON* roomTable_json = getJson(ROOM_TABLE_JSON_FILE);
    BASIC_CreateTable(&roomTable, roomTable_json, VILLA_CreateRoom);
    cJSON_Delete(roomTable_json);

    VILLA_SetCharacterCoord(characterTable.kv[0].val, (Coord){roomTable.kv[0].val, 3, 0});
    VILLA_SetCharacterCoord(characterTable.kv[0].val, (Coord){roomTable.kv[0].val, 3, 0});
    VILLA_SetCharacterCoord(characterTable.kv[1].val, (Coord){roomTable.kv[0].val, 0, 3});
    VILLA_SetCharacterCoord(characterTable.kv[1].val, (Coord){roomTable.kv[0].val, 0, 3});

    you = characterTable.kv[1].val;

    VILLA_InitRain();

    for (int i = 0; i < VILLA_NUM_DIRECTS; i++) {
        for (int j = 0; j < VILLA_NUM_ACTS; j++) {
            TEX_SRC[i][j] = (SDL_FRect){
                (float)j / VILLA_NUM_ACTS, (float)i / VILLA_NUM_DIRECTS,
                (float)1 / VILLA_NUM_ACTS, (float)1 / VILLA_NUM_DIRECTS,
            };
        }
    }
    return true;
}
static bool VILLA_Renew_Camera() {
    static Vec3f rotateSmall = {0}, rotateLarge = {0};
    const float t = (float)SDL_GetTicks() / 1000;
    {
        const float angle = M_PI_4 * 0.5;
        static Vec3f v1 = {0}, v2 = {0};
        static float t1 = 0, t2 = 0;

        rotateSmall = LOTRI_AtvVec(v1, v2, (t - t1) / (t2 - t1), BASIC_AtvRank2);

        Vec3f v = {0, 0.5f, 0.3f};
        if (PERPH_GetKeyPressed(SDL_SCANCODE_DOWN )) v.v.y += angle;
        if (PERPH_GetKeyPressed(SDL_SCANCODE_UP   )) v.v.y -= angle;
        if (PERPH_GetKeyPressed(SDL_SCANCODE_LEFT )) v.v.z += angle;
        if (PERPH_GetKeyPressed(SDL_SCANCODE_RIGHT)) v.v.z -= angle;

        if (BASIC_GetVecEqual(v, v2) == false) {
            v1 = rotateSmall;
            t1 = t;
            v2 = v;
            t2 = t + 0.8f;
        }
    }
    {
        const float angle = M_PI_4 * 0.5;
        static Vec3f v1 = {0}, v2 = {0};
        static float t1 = 0, t2 = 0;

        rotateLarge = LOTRI_AtvVec(v1, v2, (t - t1) / (t2 - t1), BASIC_AtvRank2);

        Vec3f v = {0, 0.5f, 0.3f};
        if (PERPH_GetKeyPressed(SDL_SCANCODE_Q)) cameraDirect = (cameraDirect + 1) % VILLA_NUM_DIRECTS;
        if (PERPH_GetKeyPressed(SDL_SCANCODE_E)) cameraDirect = (cameraDirect + VILLA_NUM_DIRECTS - 1) % VILLA_NUM_DIRECTS;

        if (BASIC_GetVecEqual(v, v2) == false) {
            v1 = rotateSmall;
            t1 = t;
            v2 = v;
            t2 = t + 0.8f;
        }
    }

    static DelayVec3f bigRotate = {0};
    bigRotate.block = true;
    {
        static float k = 2;
        Vec3f v = {0, 0, 0};
        if (PERPH_GetKeyPress(SDL_SCANCODE_Q)) k += 1;
        if (PERPH_GetKeyPress(SDL_SCANCODE_E)) k -= 1;
        v.v.z += k * (float)M_PI_2;
        LOTRI_SetDelayVec(&bigRotate, v, 0.5f);
    }
    const Vec3f vd = LOTRI_GetDelayVecVec(bigRotate);
    const Vec3f rotate = (Vec3f){
        rotateSmall.v.x + vd.v.x,
        rotateSmall.v.y + vd.v.y,
        rotateSmall.v.z + vd.v.z
    };
    LOTRI_SetCamera(rotate);
    return true;
}
static bool VILLA_Renew_You() {
    if (you == NULL) return false;

    if (PERPH_GetKeyPressed(SDL_SCANCODE_W)) VILLA_SetCharacterMove(you, VILLA_DIRECT_PX);
    if (PERPH_GetKeyPressed(SDL_SCANCODE_A)) VILLA_SetCharacterMove(you, VILLA_DIRECT_PY);
    if (PERPH_GetKeyPressed(SDL_SCANCODE_S)) VILLA_SetCharacterMove(you, VILLA_DIRECT_NX);
    if (PERPH_GetKeyPressed(SDL_SCANCODE_D)) VILLA_SetCharacterMove(you, VILLA_DIRECT_NY);
    return true;
}
bool VILLA_Renew() {
    return true
    && VILLA_Renew_You()
    && BASIC_RenewTable(&roomTable, VILLA_RenewRoom)
    && BASIC_RenewTable(&characterTable, VILLA_RenewCharacter)
    && VILLA_Renew_Camera()
    ;
}
bool VILLA_Draw() {
    return true
    && BASIC_DrawTable(&roomTable, VILLA_DrawRoom)
    && BASIC_DrawTable(&characterTable, VILLA_DrawCharacter)
    && VILLA_DrawRain()
    ;
}
void VILLA_Exit() {
    BASIC_DeleteTable(&characterTable, VILLA_DeleteCharacter);
    BASIC_DeleteTable(&roomTable, VILLA_DestroyRoom_V);
}
