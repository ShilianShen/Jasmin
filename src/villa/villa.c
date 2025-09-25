#include "villa.h"

#include "room.h"
#include "weather.h"
#include "character.h"


const char CHARACTER_TABLE_JSON_FILE[] = "../config/villa_character.json";
const char ROOM_TABLE_JSON_FILE[] = "../config/villa_room.json";


Table characterTable;
Table roomTable;
SDL_FRect TEX_SRC[VILLA_NUM_DIRECTS][VILLA_NUM_ACTS];


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
    static DelayVec3f smallRotate = {0}, bigRotate = {0};
    bigRotate.block = true;
    {
        const float angle = M_PI_4 * 0.5;
        Vec3f v = {0, 0.5f, 0.3f};
        if (PERPH_GetKeyPressed(SDL_SCANCODE_DOWN )) v.v.y += angle;
        if (PERPH_GetKeyPressed(SDL_SCANCODE_UP   )) v.v.y -= angle;
        if (PERPH_GetKeyPressed(SDL_SCANCODE_LEFT )) v.v.z += angle;
        if (PERPH_GetKeyPressed(SDL_SCANCODE_RIGHT)) v.v.z -= angle;
        LOTRI_SetDelayVec(&smallRotate, v, 1.f);
    }
    {
        static float k = 2;
        Vec3f v = {0, 0, 0};
        if (PERPH_GetKeyPress(SDL_SCANCODE_Q)) k += 1;
        if (PERPH_GetKeyPress(SDL_SCANCODE_E)) k -= 1;
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
