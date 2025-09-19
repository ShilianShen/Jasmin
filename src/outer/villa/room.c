#include "room.h"


struct Room {
    Model* model;
    SDL_Texture* mask;
};


// CREATE & DELETE =====================================================================================================
static bool VILLA_CreateRoom_RK(Room* room, const cJSON *room_json) {
    memset(room, 0, sizeof(*room));
    if (cJSON_IsObject(room_json) == false) {
        printf("cJSON_IsObject() failed\n");
        return false;
    }
    char* key = NULL;
    char* model_json = NULL;
    char* material_json = NULL;
    char* mask_json = NULL;
    if (cJSON_LoadFromObj(room_json, key = "model", JSM_STRING, &model_json) == false) {
        printf("%s: failed in %s\n", __func__, key);
        return false;
    }
    if (cJSON_LoadFromObj(room_json, key = "material", JSM_STRING, &material_json) == false) {
        printf("%s: failed in %s\n", __func__, key);
        return false;
    }
    if (cJSON_LoadFromObj(room_json, key = "mask", JSM_STRING, &mask_json) == false) {
        printf("%s: failed in %s\n", __func__, key);
        return false;
    }

    room->model = LOTRI_CreateModel(model_json, material_json, MODEL_SIDE_IN);
    if (room->model == NULL) {
        printf("%s: room->model == NULL\n", __func__);
        return false;
    }

    room->mask = IMG_LoadTexture(renderer, mask_json);
    if (room->mask == NULL) {
        printf("%s: room->mask == NULL\n", __func__);
        return false;
    }

    return true;
}
Room *VILLA_CreateRoom(const cJSON *room_json) {
    if (room_json == NULL) {
        printf("%s: room_json == NULL\n", __func__);
        return NULL;
    }
    Room* room = calloc(1, sizeof(Room));
    if (room == NULL) {
        printf("%s: room == NULL\n", __func__);
        return room;
    }
    if (VILLA_CreateRoom_RK(room, room_json) == false) {
        printf("%s: VILLA_CreateRoom_RK failed\n", __func__);
        VILLA_DeleteRoom(room);
    }
    return room;
}
Room* VILLA_DeleteRoom(Room *room) {
    if (room != NULL) {
        if (room->model != NULL) {
            LOTRI_DestroyModel(room->model);
            room->model = NULL;
        }
        if (room->mask != NULL) {
            SDL_DestroyTexture(room->mask);
            room->mask = NULL;
        }
        free(room);
        room = NULL;
    }
    return NULL;
}

// RENEW ===============================================================================================================
bool VILLA_RenewRoom(Room* room) {
    return true;
}

// DRAW ================================================================================================================
bool VILLA_DrawRoom(Room* room) {
    return true;
}


