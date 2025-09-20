#include "room.h"


struct Room {
    Model* model;
    SDL_Texture* mask;
    SDL_Rect mask_indices;
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
    if (cJSON_LoadFromObj(room_json, key = "rect", JSM_RECT, &room->mask_indices) == false) {
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
    SDL_SetTextureScaleMode(room->mask, SDL_SCALEMODE_NEAREST);

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
    LOTRI_RenewModel(room->model);
    return true;
}


// DRAW ================================================================================================================
bool VILLA_DrawRoom(const Room *room) {
    LOTRI_DrawModel(room->model);
    Vec3f vec3s[4];
    LOTRI_GetModelWorldVertex(room->model, room->mask_indices.x, &vec3s[0]);
    LOTRI_GetModelWorldVertex(room->model, room->mask_indices.y, &vec3s[1]);
    LOTRI_GetModelWorldVertex(room->model, room->mask_indices.w, &vec3s[2]);
    LOTRI_GetModelWorldVertex(room->model, room->mask_indices.h, &vec3s[3]);
    SDL_Vertex vertices[4] = {0};
    for (int i = 0; i < 4; i++) {
        vertices[i] = (SDL_Vertex){
            .position = (SDL_FPoint){vec3s[i].v.x, vec3s[i].v.y},
            .color = {255, 255, 255, 255}
        };
    }
    vertices[0].tex_coord = (SDL_FPoint){0, 0};
    vertices[1].tex_coord = (SDL_FPoint){1, 0};
    vertices[2].tex_coord = (SDL_FPoint){1, 1};
    vertices[3].tex_coord = (SDL_FPoint){0, 1};
    const int indices[6] = {0, 1, 2, 2, 3, 0};
    if (SDL_RenderGeometry(renderer, room->mask, vertices, 4, indices, 6) == false) {
        printf("%s: failed in %s\n", __func__, SDL_GetError());
        return false;
    }
    return true;
}


