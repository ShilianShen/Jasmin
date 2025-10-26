#include "villa.h"

#include "room.h"
#include "weather.h"
#include "character.h"
#include "message.h"


const char* CHARACTER_TABLE_JSON_FILE = "../config/villa_character.json";
const char* ROOM_TABLE_JSON_FILE = "../config/villa_room.json";


Table characterTable;
Table roomTable;
TTF_Font* VILLA_Font = NULL;
SDL_FRect TEX_SRC[VILLA_NUM_DIRECTS][VILLA_NUM_ACTS];
int cameraDirect;


Character* you = NULL;


enum {CAMERA_Q, CAMERA_E, CAMERA_W, CAMERA_A, CAMERA_S, CAMERA_D, NUM_CAMERA_MOVES};
static bool cameraMoves[NUM_CAMERA_MOVES];


// TRIG ================================================================================================================
static void VILLA_TrigMoveYou(const TrigPara para) {
    const int direct = (int)para;
    VILLA_SetCharacterMove(you, direct);
}
static void VILLA_TrigMoveCamera(const TrigPara para) {
    cameraMoves[para] = true;
}
static void VILLA_TrigSendMessage(const TrigPara para) {
    VILLA_SendMessage("This is a text for test.");
}
static void VILLA_TrigClearMessage(const TrigPara para) {
    VILLA_ResetMessage();
}


// INIT & EXIT =========================================================================================================
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
    VILLA_InitMessage();

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
void VILLA_Exit() {
    BASIC_DeleteTable(&characterTable, VILLA_DeleteCharacter);
    BASIC_DeleteTable(&roomTable, VILLA_DestroyRoom_V);
    VILLA_ExitMessage();
}


// RENEW ===============================================================================================================
static bool VILLA_Renew_Camera() {
    Vec3f position;
    VILLA_GetCharacterPosition(you, &position);

    static Vec3f rotateSmall = {0}, rotateLarge = {0};
    const float t = (float)SDL_GetTicks() / 1000;
    {
        const float angle = M_PI_4 * 0.5;
        static Vec3f v1 = {0}, v2 = {0};
        static float t1 = 0, t2 = 0;

        rotateSmall = LOTRI_AtvVec(v1, v2, (t - t1) / (t2 - t1), BASIC_AtvRank2);

        Vec3f v = {0, 0.5f, 0.3f};
        if (cameraMoves[CAMERA_S]) v.v.y += angle;
        if (cameraMoves[CAMERA_W]) v.v.y -= angle;
        if (cameraMoves[CAMERA_A]) v.v.z += angle;
        if (cameraMoves[CAMERA_D]) v.v.z -= angle;

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
            if (cameraMoves[CAMERA_Q]) direct++;
            if (cameraMoves[CAMERA_E]) direct--;
            const Vec3f v = {0, 0, M_PI_2 * direct};
            if (BASIC_GetVecEqual(v, v2) == false) {
                v1 = rotateLarge;
                t1 = t;
                v2 = v;
                t2 = t + 0.4f;
            }
        }
    }
    const Vec3f rotation = BASIC_GetAdd(rotateSmall, rotateLarge);
    LOTRI_SetCamera(rotation, position);
    const float z = SDL_fmodf(SDL_fmodf(rotation.v.z, 2 * M_PI) + 2 * M_PI, 2 * M_PI);
    if (7 * M_PI_4 <= z || z < 1 * M_PI_4) cameraDirect = VILLA_DIRECT_PX;
    else if (1 * M_PI_4 <= z && z < 3 * M_PI_4) cameraDirect = VILLA_DIRECT_PY;
    else if (3 * M_PI_4 <= z && z < 5 * M_PI_4) cameraDirect = VILLA_DIRECT_NX;
    else if (5 * M_PI_4 <= z && z < 7 * M_PI_4) cameraDirect = VILLA_DIRECT_NY;
    DEBUG_SendMessageL("%s: %.2f, %d\n", __func__, z, cameraDirect);

    for (int i = 0; i < NUM_CAMERA_MOVES; i++) cameraMoves[i] = false;

    return true;
}
static bool VILLA_Renew_Trig() {
    if (VILLA_GetForwardCharacter(you) != NULL) {
        if (PERPH_GetKeyPressed(SDL_SCANCODE_RSHIFT)) {
            PERPH_SetKeyTrig(SDL_SCANCODE_RETURN, (Trig){VILLA_TrigClearMessage, 0, false});
        }
        else {
            PERPH_SetKeyTrig(SDL_SCANCODE_RETURN, (Trig){VILLA_TrigSendMessage, 0, false});
        }
    }
    if (you != NULL && VILLA_HaveMessage() == false) {
        PERPH_SetKeyTrig(SDL_SCANCODE_W, (Trig){VILLA_TrigMoveYou, VILLA_DIRECT_PX + cameraDirect, true});
        PERPH_SetKeyTrig(SDL_SCANCODE_A, (Trig){VILLA_TrigMoveYou, VILLA_DIRECT_PY + cameraDirect, true});
        PERPH_SetKeyTrig(SDL_SCANCODE_S, (Trig){VILLA_TrigMoveYou, VILLA_DIRECT_NX + cameraDirect, true});
        PERPH_SetKeyTrig(SDL_SCANCODE_D, (Trig){VILLA_TrigMoveYou, VILLA_DIRECT_NY + cameraDirect, true});
    }

    PERPH_SetKeyTrig(SDL_SCANCODE_UP, (Trig){VILLA_TrigMoveCamera, CAMERA_W, true});
    PERPH_SetKeyTrig(SDL_SCANCODE_LEFT, (Trig){VILLA_TrigMoveCamera, CAMERA_A, true});
    PERPH_SetKeyTrig(SDL_SCANCODE_DOWN, (Trig){VILLA_TrigMoveCamera, CAMERA_S, true});
    PERPH_SetKeyTrig(SDL_SCANCODE_RIGHT, (Trig){VILLA_TrigMoveCamera, CAMERA_D, true});
    PERPH_SetKeyTrig(SDL_SCANCODE_Q, (Trig){VILLA_TrigMoveCamera, CAMERA_Q, true});
    PERPH_SetKeyTrig(SDL_SCANCODE_E, (Trig){VILLA_TrigMoveCamera, CAMERA_E, true});

    return true;
}
bool VILLA_Renew() {
    return true
    && VILLA_Renew_Trig()
    && BASIC_RenewTable(&roomTable, VILLA_RenewRoom)
    && BASIC_RenewTable(&characterTable, VILLA_RenewCharacter)
    && VILLA_Renew_Camera()
    && VILLA_RenewMessage()
    ;
}


// DRAW ================================================================================================================
bool VILLA_Draw() {
    return true
    && BASIC_DrawTable(&roomTable, VILLA_DrawRoom)
    && BASIC_DrawTable(&characterTable, VILLA_DrawCharacter)
    && LOTRI_Draw()
    // && INTEL_Draw()
    && VILLA_DrawMessage()
    ;
}

