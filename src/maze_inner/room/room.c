#include "room.h"


const SDL_FRect ROOM_DEFAULT_GID_RECT = {0, 0, 16, 16};
bool SDL_CompareSDLColor(const SDL_Color x, const SDL_Color y) {
    return x.r == y.r && x.g == y.g && x.b == y.b && x.a == y.a;
}


const SDL_Color ROOM_DEFAULT_COLOR = {0, 0, 0, 0};
void** allocate2DArray(size_t w, size_t h, size_t elementSize) {
    void** array = (void**)malloc(w * sizeof(void*));
    if (array == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < w; i++) {
        array[i] = malloc(h * elementSize);
        if (array[i] == NULL) {
            // 分配失败，释放之前已分配的内存
            for (size_t j = 0; j < i; j++) {
                free(array[j]);
            }
            free(array);
            return NULL;
        }
    }

    return array;
}
void free2DArray(void** array, size_t w) {
    if (array == NULL) return;

    for (size_t i = 0; i < w; i++) {
        free(array[i]);
    }
    free(array);
}
bool loadStringFromSDLColor(char* string, const SDL_Color color) {
    if (string == NULL) {
        printf("%s: string is null.\n", __func__);
        return false;
    }
    // 00000000 - FFFFFFFF
    if (strlen(string) < 8) {
        printf("%s: string is too short, %lu.\n", __func__, strlen(string));
        return false;
    }
    snprintf(string, 8, "%02X%02X%02X%02X", color.r, color.g, color.b, color.a);
    return true;
}
static void ROOM_SetNetCenter(const char* para);
SDL_FRect EASE_GetFRect(const SDL_FRect rect1, const SDL_FRect rect2, const float t) {
    SDL_FRect rect;
    rect.x = (1 - t) * rect1.x + t * rect2.x;
    rect.y = (1 - t) * rect1.y + t * rect2.y;
    rect.w = (1 - t) * rect1.w + t * rect2.w;
    rect.h = (1 - t) * rect1.h + t * rect2.h;
    return rect;
}


// BLOCK NET ===========================================================================================================
Direction** roomNet;
static int roomNetCenter = 0;


static void ROOM_LoadRoomNet() {
    roomNet = (Direction**)allocate2DArray(lenRoomSet, lenRoomSet, sizeof(Direction));
    if (roomNet == NULL) {
        printf("%s.\n", __func__);
        return;
    } // Req Condition
    for (int i = 0; i < lenRoomSet; i++) {
        for (int j = 0; j < lenRoomSet; j++) {
            roomNet[i][j] = DIRECTION_ILLEGAL;
            for (int k = 0; k < NUM_DIRECTIONS; k++) {
                const SDL_Color color1 = roomSet[i].gateColors[k];
                const SDL_Color color2 = roomSet[j].color;
                if (SDL_CompareSDLColor(color1, color2) == true) {
                    roomNet[i][j] = k;
                    break;
                }
            }
        }
    }
    for (int i = 0; i < lenRoomSet; i++) {
        for (int j = 0; j < lenRoomSet; j++) {
            switch (roomNet[i][j]) {
                case DIRECTION_W: {
                    if (roomNet[j][i] != DIRECTION_S) {
                        roomNet[i][j] = DIRECTION_ILLEGAL;
                    }
                    break;
                }
                case DIRECTION_A: {
                    if (roomNet[j][i] != DIRECTION_D) {
                        roomNet[i][j] = DIRECTION_ILLEGAL;
                    }
                    break;
                }
                case DIRECTION_S: {
                    if (roomNet[j][i] != DIRECTION_W) {
                        roomNet[i][j] = DIRECTION_ILLEGAL;
                    }
                    break;
                }
                case DIRECTION_D: {
                    if (roomNet[j][i] != DIRECTION_A) {
                        roomNet[i][j] = DIRECTION_ILLEGAL;
                    }
                    break;
                }
                default: break;
            }
        }
    }
}
static void ROOM_UnloadRoomNet() {
    if (roomNet != NULL) {
        free2DArray((void**)roomNet, lenRoomSet);
        roomNet = NULL;
    }
}
static int ROOM_GetRoomFromNet(const int i, const Direction gate) {
    for (int j = 0; j < lenRoomSet; j++) {
        if (roomNet[i][j] == gate) {
            return j;
        }
    }
    return i;
}


// BLOCK COVER =========================================================================================================
static SDL_Texture* cover[NUM_DIRECTIONS];
static void ROOM_LoadRoomCover() {
    cover[DIRECTION_W] = IMG_LoadTexture(mazeRenderer, "../images/blocks/coverW.png");
    cover[DIRECTION_A] = IMG_LoadTexture(mazeRenderer, "../images/blocks/coverA.png");
    cover[DIRECTION_S] = IMG_LoadTexture(mazeRenderer, "../images/blocks/coverS.png");
    cover[DIRECTION_D] = IMG_LoadTexture(mazeRenderer, "../images/blocks/coverD.png");
    for (int i = 0; i < NUM_DIRECTIONS; i++) {
        SDL_SetTextureScaleMode(cover[i], SDL_SCALEMODE_NEAREST);
    }
}
static void ROOM_UnloadRoomCover() {
    for (int i = 0; i < NUM_DIRECTIONS; i++) {
        SDL_DestroyTexture(cover[i]);
    }
}


// BLOCK ===============================================================================================================
static void ROOM_LoadRoom(Room* room, const char* path) {
    // Req Condition
    if (room == NULL) {
        printf("%s: block not exists.\n", __func__);
        return;
    }
    if (path == NULL) {
        printf("%s: path not exists.\n", __func__);
        return;
    }
    SDL_Surface* surface = IMG_Load(path); // malloc
    if (surface == NULL) {
        printf("%s: failed to load texture.\n", __func__);
        return;
    }
    //
    *room = (Room){0};

    // load w, h
    room->w = surface->w;
    room->h = surface->h;
    room->surface = surface;

    // load wall
    room->wall = (bool**)allocate2DArray(surface->w, surface->h, sizeof(bool)); // malloc
    for (int i = 0; i < surface->w; i++) {
        for (int j = 0; j < surface->h; j++) {
            room->wall[i][j] = true;
        }
    }
    for (int i = 1; i < surface->w-1; i++) {
        for (int j = 1; j < surface->h-1; j++) {
            SDL_Color color = {0};
            SDL_ReadSurfaceSDLColor(surface, i, j, &color);
            if (SDL_CompareSDLColor(color, (SDL_Color){255, 255, 255, 255}) == true) {
                room->wall[i][j] = false;
            }
            else {
                const Gene* gene = GENE_FindGeneFromColor(color);
                if (gene == NULL) {
                    continue;
                }
                const char* name = GENE_GetNameFromGene(gene);
                if (name == NULL) {
                    continue;
                }
                if (strcmp(name, "You") == 0) {
                    room->wall[i][j] = false;
                }
            }
        }
    }

    // load color
    SDL_ReadSurfaceSDLColor(surface, 0, 0, &room->color);

    // load elem
    char para[] = "00FF00FF";
    loadStringFromSDLColor(para, room->color);
    Elem* elem = TEMPO_CreateElem(
        ELEM_TYPE_FILE,
        path,
        40,
        &ROOM_DEFAULT_GID_RECT,
        &ROOM_SetNetCenter,
        para); // malloc
    if (elem == NULL) {
        printf("%s: failed to create elem.\n", __func__);
        return;
    }
    room->elem = elem;

    // load gate
    // W
    for (int i = 1, j = 0; i < surface->w-1; i++) {
        SDL_Color gateColor = ROOM_DEFAULT_COLOR;
        SDL_ReadSurfaceSDLColor(surface, i, j, &gateColor);
        if (SDL_CompareSDLColor(gateColor, ROOM_DEFAULT_COLOR) == false
        && SDL_CompareSDLColor(gateColor, room->color) == false) {
            room->gateColors[DIRECTION_W] = gateColor;
        }
    }
    // A
    for (int i = 0, j = 1; j < surface->h-1; j++) {
        SDL_Color gateColor = ROOM_DEFAULT_COLOR;
        SDL_ReadSurfaceSDLColor(surface, i, j, &gateColor);
        if (SDL_CompareSDLColor(gateColor, ROOM_DEFAULT_COLOR) == false
        && SDL_CompareSDLColor(gateColor, room->color) == false) {
            room->gateColors[DIRECTION_A] = gateColor;
        }
    }
    // S
    for (int i = 1, j = surface->h-1 ; i < surface->w-1; i++) {
        SDL_Color gateColor = ROOM_DEFAULT_COLOR;
        SDL_ReadSurfaceSDLColor(surface, i, j, &gateColor);
        if (SDL_CompareSDLColor(gateColor, ROOM_DEFAULT_COLOR) == false
        && SDL_CompareSDLColor(gateColor, room->color) == false) {
            room->gateColors[DIRECTION_S] = gateColor;
        }
    }
    // D
    for (int i = surface->w-1, j = 1; j < surface->h-1; j++) {
        SDL_Color gateColor = ROOM_DEFAULT_COLOR;
        SDL_ReadSurfaceSDLColor(surface, i, j, &gateColor);
        if (SDL_CompareSDLColor(gateColor, ROOM_DEFAULT_COLOR) == false
        && SDL_CompareSDLColor(gateColor, room->color) == false) {
            room->gateColors[DIRECTION_D] = gateColor;
        }
    }
    //
}
static void ROOM_UnloadRoom(Room* room) {
    if (room == NULL) {
        return;
    }
    if (room->wall != NULL) {
        free2DArray((void**)room->wall, room->w);
        room->wall = NULL;
    }
    if (room->elem != NULL) {
        TEMPO_DestroyElem(room->elem);
        room->elem = NULL;
    }
    if (room->surface != NULL) {
        SDL_DestroySurface(room->surface); // free
        room->surface = NULL;
    }
}


// BLOCK SET ===========================================================================================================
int lenRoomSet;
Room* roomSet;
const char* roomSetPath = "../config/maze/roomSet.toml";
static void ROOM_LoadRoomSet() {
    // Req Condition
    if (roomSetPath == NULL) {
        printf("%s.\n", __func__);
        return;
    }

    toml_table_t* tomlFile = getToml(roomSetPath); // malloc
    if (tomlFile == NULL) {
        printf("%s.\n", __func__);
        return;
    }

    const toml_array_t* tomlBlocks = toml_array_in(tomlFile, "blockSet");
    if (tomlBlocks == NULL) {
        printf("%s.\n", __func__);
        return;
    }

    //
    lenRoomSet = toml_array_nelem(tomlBlocks);
    roomSet = malloc(lenRoomSet * sizeof(Room)); // malloc
    for (int i = 0; i < lenRoomSet; i++) {
        // Req Condition
        const toml_datum_t tomlBlockPath = toml_string_at(tomlBlocks, i);
        if (tomlBlockPath.ok == false) {
            printf("%s.\n", __func__);
            continue;
        }
        //
        ROOM_LoadRoom(&roomSet[i], tomlBlockPath.u.s); // malloc
    }
    toml_free(tomlFile); // free
}
static void ROOM_UnloadRoomSet() {
    if (roomSet != NULL) {
        for (int i = 0; i < lenRoomSet; i++) {
            ROOM_UnloadRoom(&roomSet[i]); // free
        }
        free(roomSet); // free
        roomSet = NULL;
        lenRoomSet = 0;
    }
}


// BLOCK DRAW INFO =====================================================================================================
static Uint64 time_start = 0, time_end = 0;
static Uint64 period = 1000;
typedef struct RoomDrawInfo {
    SDL_FRect dst_rect[2];
    int depth;
    SDL_FRect dst_rects[NUM_DIRECTIONS][2];
    int depths[NUM_DIRECTIONS];
} RDI;
RDI* RDISet;
float rate;
static void ROOM_LoadRDISet() {
    RDISet = malloc(lenRoomSet * sizeof(RDI));
    if (RDISet == NULL) {
        printf("%s.\n", __func__);
        return;
    }
    for (int i = 0; i < lenRoomSet; i++) {
        RDISet[i] = (RDI){0};
    }
}
static void ROOM_UnloadRDISet() {
    if (RDISet != NULL) {
        free(RDISet);
        RDISet = NULL;
    }
}


static void ROOM_RenewRDISet() {
    if (SDL_GetTicks() > time_end) {
        for (int i = 0; i < lenRoomSet; i++) {
            RDISet[i].dst_rect[0] = RDISet[i].dst_rect[1];
            for (int j = 0; j < NUM_DIRECTIONS; j++) {
                RDISet[i].dst_rects[j][0] = RDISet[i].dst_rects[j][1];
            }
        }
        rate = 1;
        return;
    }
    rate = EASE_Sin2((float)(SDL_GetTicks() - time_start) / (float)period);
}


// TRIG ================================================================================================================
static void ROOM_SetNetCenter(const char* para) {
    if (para == NULL) {
        return;
    }
    for (int i = 0; i < NUM_DIRECTIONS; i++) {
        char string[] = "0123456789";
        loadStringFromSDLColor(string, roomSet[i].color);
        if (roomNetCenter != i && strcmp(para, string) == 0) {
            roomNetCenter = i;
            time_start = SDL_GetTicks();
            time_end = time_start + period;
            return;
        }
    }
}


// LOAD ================================================================================================================
void ROOM_Load() {
    ROOM_LoadRoomCover();
    ROOM_LoadRoomSet();
    ROOM_LoadRoomNet();
    ROOM_LoadRDISet();
}
void ROOM_Unload() {
    ROOM_UnloadRDISet();
    ROOM_UnloadRoomCover();
    ROOM_UnloadRoomNet();
    ROOM_UnloadRoomSet();
}


// RENEW ===============================================================================================================
void ROOM_Renew_DstRect(const SDL_FRect dst_rect1, const Direction gate, SDL_FRect* dst_rect2) {
    if (dst_rect2 == NULL) {
        printf("%s.\n", __func__);
        return;
    }
    float dx = 0, dy = 0;
    switch (gate) {
        case DIRECTION_W: {
            dy = -dst_rect2->h;
            break;
        }
        case DIRECTION_A: {
            dx = -dst_rect2->w;
            break;
        }
        case DIRECTION_S: {
            dy = dst_rect1.h;
            break;
        }
        case DIRECTION_D: {
            dx = dst_rect1.w;
            break;
        }
        default: {
            printf("%s.\n", __func__);
            return;
        }
    }
    dst_rect2->x = dst_rect1.x + dx;
    dst_rect2->y = dst_rect1.y + dy;
}
void ROOM_Renew_Block() {}
void ROOM_Renew() {

    ROOM_RenewRDISet();

    if (0 > roomNetCenter || roomNetCenter >= lenRoomSet) {
        printf("%s.\n", __func__);
        return;
    }
    bool need_renew = true;

    int find[lenRoomSet];
    enum {NOT_FOUND, FOUND_NOW, HAD_FOUND};
    for (int i = 0; i < lenRoomSet; i++) {
        find[i] = NOT_FOUND;
    }
    find[roomNetCenter] = FOUND_NOW;

    TEMPO_SetElemGidRect(roomSet[roomNetCenter].elem, ROOM_DEFAULT_GID_RECT);
    TEMPO_RenewElem(roomSet[roomNetCenter].elem);
    // TEMPO_GetElemDstRect(blockSet[blockNetCenter].elem, &blockSet[blockNetCenter].dst_rect);
    TEMPO_GetElemDstRect(roomSet[roomNetCenter].elem, &RDISet[roomNetCenter].dst_rect[1]);

    int depth;
    for (int i = 0; i < lenRoomSet; i++) {
        RDISet[i].depth = 0;
        for (int j = 0; j < NUM_DIRECTIONS; j++) {
            RDISet[i].depths[j] = 0;
        }
    }
    RDISet[roomNetCenter].depth = depth = 1;

    while (need_renew) {
        depth++;
        for (int i = 0; i < lenRoomSet; i++) {
            if (find[i] != FOUND_NOW) {
                continue;
            }
            find[i] = HAD_FOUND;
            for (int j = 0; j < NUM_DIRECTIONS; j++) {
                const int idx = ROOM_GetRoomFromNet(i, j);
                if (idx == i) {
                    continue;
                }
                switch (find[idx]) {
                    case NOT_FOUND: {
                        // renew dst, depth by blockSet[i], j
                        TEMPO_RenewElem(roomSet[idx].elem);
                        TEMPO_GetElemDstRect(roomSet[idx].elem, &RDISet[idx].dst_rect[1]);
                        ROOM_Renew_DstRect(RDISet[i].dst_rect[1], j, &RDISet[idx].dst_rect[1]);
                        find[idx] = FOUND_NOW;
                        RDISet[idx].depth = depth;
                        break;
                    }
                    case FOUND_NOW:
                    case HAD_FOUND: {
                        // renew gate.dst, gate.depth by blockSet[i], j
                        break;
                    }
                    default: break;
                }
                TEMPO_GetElemDstRect(roomSet[idx].elem, &RDISet[idx].dst_rects[j][1]);
                ROOM_Renew_DstRect(RDISet[i].dst_rect[1], j, &RDISet[idx].dst_rects[j][1]);
                RDISet[idx].depths[j] = depth;
            }
        } // 遍历find
        need_renew = false;
        for (int i = 0; i < lenRoomSet; i++) {
            if (find[i] == FOUND_NOW) {
                need_renew = true;
                break;
            }
        }
    }
}


// DRAW ================================================================================================================
void ROOM_Draw_BDI() {
    for (int i = 0; i < lenRoomSet; i++) {
        for (int j = 0; j < NUM_DIRECTIONS; j++) {
            if (RDISet[i].depths[j] < 0) {
                continue;
            }
            const SDL_FRect dst_rect = EASE_GetFRect(RDISet[i].dst_rects[j][0], RDISet[i].dst_rects[j][1], rate);
            TEMPO_SetElemDstRect(roomSet[i].elem, dst_rect);
            TEMPO_DrawElem(roomSet[i].elem);
        }
    }
    for (int i = 0; i < lenRoomSet; i++) {
        for (int j = 0; j < NUM_DIRECTIONS; j++) {
            if (RDISet[i].depths[j] < 0) {
                continue;
            }
            const SDL_FRect dst_rect = EASE_GetFRect(RDISet[i].dst_rects[j][0], RDISet[i].dst_rects[j][1], rate);
            SDL_RenderTexture(mazeRenderer, cover[j], NULL, &dst_rect);
        }
    }
    for (int i = 0; i < lenRoomSet; i++) {
        if (RDISet[i].depth > 0) {
            const SDL_FRect dst_rect = EASE_GetFRect(RDISet[i].dst_rect[0], RDISet[i].dst_rect[1], rate);
            TEMPO_SetElemDstRect(roomSet[i].elem, dst_rect);
            TEMPO_DrawElem(roomSet[i].elem);
        }
    }
}

