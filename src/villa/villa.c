#include "villa.h"

#include "room.h"
#include "weather.h"
#include "character.h"


const char CHARACTER_TABLE_JSON_FILE[] = "../config/villa_character.json";
const char ROOM_TABLE_JSON_FILE[] = "../config/villa_room.json";


Table characterTable;
Table roomTable;
TTF_Font* VILLA_Font = NULL;
SDL_FRect TEX_SRC[VILLA_NUM_DIRECTS][VILLA_NUM_ACTS];
int cameraDirect;


Character* you = NULL;


void VILLA_MoveYou(void* direct_void) {
    const int direct = *(int*)direct_void;
    VILLA_SetCharacterMove(you, direct);
}
void VILLA_MoveCameraSmall(const void* para_void) {


}


bool VILLA_Init() {
    cJSON* characterTable_json = getJson(CHARACTER_TABLE_JSON_FILE);
    BASIC_CreateTable(&characterTable, characterTable_json, VILLA_CreateCharacter);
    cJSON_Delete(characterTable_json);

    cJSON* roomTable_json = getJson(ROOM_TABLE_JSON_FILE);
    BASIC_CreateTable(&roomTable, roomTable_json, VILLA_CreateRoom);
    cJSON_Delete(roomTable_json);

    VILLA_SetCharacterCoord(characterTable.kv[0].val, (Coord){roomTable.kv[0].val, 3, 0});
    VILLA_SetCharacterCoord(characterTable.kv[1].val, (Coord){roomTable.kv[0].val, 2, 4});

    you = characterTable.kv[0].val;

    VILLA_InitRain();

    VILLA_Font = TTF_OpenFont("../res/font/Times New Roman.ttf", 60);
    REQ_CONDITION(VILLA_Font != NULL, return false);

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
        static int direct = 0;
        static Vec3f v1 = {0}, v2 = {0};
        static float t1 = 0, t2 = 0;
        rotateLarge = LOTRI_AtvVec(v1, v2, (t - t1) / (t2 - t1), BASIC_AtvRank2);
        if (t >= t2){
            if (PERPH_GetKeyPressed(SDL_SCANCODE_Q)) direct++;
            if (PERPH_GetKeyPressed(SDL_SCANCODE_E)) direct--;
            const Vec3f v = {0, 0, M_PI_2 * direct};
            if (BASIC_GetVecEqual(v, v2) == false) {
                v1 = rotateLarge;
                t1 = t;
                v2 = v;
                t2 = t + 0.4f;
            }
        }
    }
    const Vec3f cameraRotate = BASIC_GetAdd(rotateSmall, rotateLarge);
    LOTRI_SetCamera(cameraRotate);
    const float z = SDL_fmodf(SDL_fmodf(cameraRotate.v.z, 2 * M_PI) + 2 * M_PI, 2 * M_PI);
    if (7 * M_PI_4 <= z || z < 1 * M_PI_4) cameraDirect = VILLA_DIRECT_PX;
    else if (1 * M_PI_4 <= z && z < 3 * M_PI_4) cameraDirect = VILLA_DIRECT_PY;
    else if (3 * M_PI_4 <= z && z < 5 * M_PI_4) cameraDirect = VILLA_DIRECT_NX;
    else if (5 * M_PI_4 <= z && z < 7 * M_PI_4) cameraDirect = VILLA_DIRECT_NY;
    DEBUG_SendMessageL("%s: %.2f, %d\n", __func__, z, cameraDirect);

    return true;
}
static bool VILLA_Renew_You() {
    REQ_CONDITION(you != NULL, return false);
    static int directs[VILLA_NUM_DIRECTS];
    static Trig trig[VILLA_NUM_DIRECTS];
    for (int i = 0; i < VILLA_NUM_DIRECTS; i++) {
        trig[i] = (Trig){VILLA_MoveYou, &directs[i], true},
        directs[i] = i + cameraDirect;
    }
    PERPH_SetKeyTrig(SDL_SCANCODE_W, trig[VILLA_DIRECT_PX]);
    PERPH_SetKeyTrig(SDL_SCANCODE_A, trig[VILLA_DIRECT_PY]);
    PERPH_SetKeyTrig(SDL_SCANCODE_S, trig[VILLA_DIRECT_NX]);
    PERPH_SetKeyTrig(SDL_SCANCODE_D, trig[VILLA_DIRECT_NY]);
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
    && LOTRI_Draw()
    && VILLA_Ask(NULL, NULL)
    ;
}
void VILLA_Exit() {
    BASIC_DeleteTable(&characterTable, VILLA_DeleteCharacter);
    BASIC_DeleteTable(&roomTable, VILLA_DestroyRoom_V);
}
