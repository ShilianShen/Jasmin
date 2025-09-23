#include "room.h"


const SDL_Color colors[VILLA_NUM_DATA_TYPES] = {
    [VILLA_DATA_NONE] = {255, 255, 255, 255},
    [VILLA_DATA_ROOM] = {255, 0, 0, 255},
    [VILLA_DATA_CHARACTER] = {0, 255, 0, 255},
    [VILLA_DATA_WALL] = {0, 0, 0, 255},
};
typedef struct RoomCell RoomCell;
struct Room {
    Model* model;
    SDL_Surface* maskSur;
    SDL_Texture* maskTex;
    SDL_Rect mask_indices;
    int w, h;
    RoomCell** cells;
};
struct RoomCell {
    VILLA_DATA_TYPE dataType;
    const void* dataPtr;
    Vec3f worldPosition;
    Vec3f cameraRotation;
    Vec3f cameraPosition;
};



// SET & GET ===========================================================================================================
bool VILLA_GetRoomCellEmpty(const Coord coord) {
    return
    coord.room != NULL
    && 0 <= coord.x && coord.x < coord.room->w
    && 0 <= coord.y && coord.y < coord.room->h
    && coord.room->cells[coord.x][coord.y].dataType == VILLA_DATA_NONE
    ;
}
bool VILLA_GetRoomCellPosition(const Coord coord, Vec3f* position) {
    if (coord.room == NULL) {
        printf("%s: room == NULL.\n", __func__);
        return false;
    }
    if (position == NULL) {
        printf("%s: position == NULL.\n", __func__);
        return false;
    }
    if (0 <= coord.x && coord.x < coord.room->w && 0 <= coord.y && coord.y < coord.room->h == false) {
        printf("%s: 0 <= x && x < room->w && 0 <= y && y < room->h == false.\n", __func__);
        return false;
    }
    *position = coord.room->cells[coord.x][coord.y].worldPosition;
    return true;
}


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

    if (cJSON_Load(room_json, key = "model", JSM_STRING, &model_json) == false) {
        printf("%s: failed in %s\n", __func__, key);
        return false;
    }
    if (cJSON_Load(room_json, key = "material", JSM_STRING, &material_json) == false) {
        printf("%s: failed in %s\n", __func__, key);
        return false;
    }
    if (cJSON_Load(room_json, key = "mask", JSM_STRING, &mask_json) == false) {
        printf("%s: failed in %s\n", __func__, key);
        return false;
    }
    if (cJSON_Load(room_json, key = "rect", JSM_RECT, &room->mask_indices) == false) {
        printf("%s: failed in %s\n", __func__, key);
        return false;
    }

    room->model = LOTRI_CreateModel(model_json, material_json, MODEL_SIDE_IN);
    if (room->model == NULL) {
        printf("%s: room->model == NULL\n", __func__);
        return false;
    }

    room->maskSur = IMG_Load(mask_json);
    if (room->maskSur == NULL) {
        printf("%s: IMG_Load() failed\n", __func__);
        return false;
    }
    room->w = room->maskSur->w;
    room->h = room->maskSur->h;

    room->maskTex = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,
        room->w, room->h
        );
    if (room->maskTex == NULL) {
        printf("%s: room->maskTex == NULL\n", __func__);
        return false;
    }
    SDL_SetTextureScaleMode(room->maskTex, SDL_SCALEMODE_NEAREST);

    room->cells = (RoomCell**)allocate2DArray(room->w, room->h, sizeof(RoomCell));
    if (room->cells == NULL) {
        printf("%s: room->cells == NULL\n", __func__);
        return false;
    }
    for (int i = 0; i < room->w; i++) {
        for (int j = 0; j < room->h; j++) {
            SDL_Color color;
            if (SDL_GetSurfaceColor(room->maskSur, i, j, &color) == false) {
                printf("%s: failed in %s\n", __func__, key);
                return false;
            }
            if (SDL_GetColorEqual(color, colors[VILLA_DATA_NONE]) == false) {
                room->cells[i][j].dataType = VILLA_DATA_WALL;
            }
            else {
                room->cells[i][j].dataType = VILLA_DATA_NONE;
            }
        }
    }

    return true;
}
void *VILLA_CreateRoom(const cJSON *room_json) {
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
void VILLA_DestroyRoom_V(void* room_void) {
    Room* room = room_void;
    VILLA_DeleteRoom(room);
}
void VILLA_DeleteRoom(void *room_void) {
    Room* room = room_void;
    if (room != NULL) {
        if (room->model != NULL) {
            LOTRI_DestroyModel(room->model);
            room->model = NULL;
        }
        if (room->maskSur != NULL) {
            SDL_DestroySurface(room->maskSur);
            room->maskSur = NULL;
        }
        if (room->maskTex != NULL) {
            SDL_DestroyTexture(room->maskTex);
            room->maskTex = NULL;
        }
        if (room->cells != NULL) {
            free2DArray((void**)room->cells, room->w);
        }
        free(room);
        room = NULL;
    }
}


// RENEW ===============================================================================================================
static bool VILLA_RenewRoomCells(Room* room) {
    const int a = room->mask_indices.x, b = room->mask_indices.y, c = room->mask_indices.h;
    Vec3f A, B, C;
    LOTRI_GetModelModelVertex(room->model, a, &A);
    LOTRI_GetModelModelVertex(room->model, b, &B);
    LOTRI_GetModelModelVertex(room->model, c, &C);

    SDL_SetRenderTarget(renderer, room->maskTex);
    for (int i = 0; i < room->w; i++) {
        for (int j = 0; j < room->h; j++) {
            RoomCell* cell = &room->cells[i][j];
            SDL_SetRenderColor(renderer, colors[cell->dataType]);
            SDL_RenderPoint(renderer, (float)i, (float)j);


            for (int k = 0; k < 3; k++) {
                cell->worldPosition.arr[k]
                = A.arr[k]
                + (B.arr[k] - A.arr[k]) * ((float)i + 0.5f) / (float)room->w
                + (C.arr[k] - A.arr[k]) * ((float)j + 0.5f) / (float)room->h;
            }
        }
    }
    SDL_SetRenderTarget(renderer, NULL);
    return true;
}
bool VILLA_RenewRoom(void *room_void) {
    Room* room = room_void;
    LOTRI_RenewModel(room->model);
    VILLA_RenewRoomCells(room);
    return true;
}


// DRAW ================================================================================================================
bool VILLA_DrawRoom(const void *room_void) {
    const Room* room = room_void;
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

    if (SDL_RenderGeometry(renderer, room->maskTex, vertices, 4, indices, 6) == false) {
        printf("%s: failed in %s\n", __func__, SDL_GetError());
        return false;
    }
    return true;
}

