#include "room.h"


const SDL_FRect ROOM_DEFAULT_GID_RECT = {0, 0, 16, 16};
bool SDL_CompareSDLColor(const SDL_Color x, const SDL_Color y) {
    return x.r == y.r && x.g == y.g && x.b == y.b && x.a == y.a;
}
const SDL_Color ROOM_DEFAULT_COLOR = {0, 0, 0, 0};
static void ROOM_SetNetCenter(const char* para);
const int SET_ILLEGAL_INDEX = -1;



// ROOM NET ============================================================================================================
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
    return SET_ILLEGAL_INDEX;
}


// ROOM COVER ==========================================================================================================
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


// ROOM ================================================================================================================
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


// ROOM SET ============================================================================================================
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


// ROOM DRAW INFO ======================================================================================================
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
static const int DEPTH_ILLEGAL = -1;
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


static void ROOM_Renew_RDISet_rate() {
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
void ROOM_Renew_Room() {}
void ROOM_Renew_RDISet_depth() {
    for (int i = 0; i < lenRoomSet; i++) {
        RDISet[i].depth = 0;
    }
    RDISet[roomNetCenter].depth = 1;
}
void ROOM_Renew() {

    ROOM_Renew_RDISet_rate();

    if (0 > roomNetCenter || roomNetCenter >= lenRoomSet) {
        printf("%s.\n", __func__);
        return;
    }
    // init
    enum {NOT_FOUND, FOUND_NOW, HAD_FOUND};
    int find_state[lenRoomSet];
    for (int i = 0; i < lenRoomSet; i++) {
        find_state[i] = NOT_FOUND;
    }
    find_state[roomNetCenter] = FOUND_NOW;

    for (int i = 0; i < lenRoomSet; i++) {
        RDISet[i].depth = DEPTH_ILLEGAL;
        for (int j = 0; j < NUM_DIRECTIONS; j++) {
            RDISet[i].depths[j] = DEPTH_ILLEGAL;
        }
    }
    RDISet[roomNetCenter].depth = 0;

    TEMPO_SetElemGidRect(roomSet[roomNetCenter].elem, ROOM_DEFAULT_GID_RECT);
    TEMPO_RenewElem(roomSet[roomNetCenter].elem);
    TEMPO_GetElemDstRect(roomSet[roomNetCenter].elem, &RDISet[roomNetCenter].dst_rect[1]);


    for (int depth = 1; depth < lenRoomSet; depth++) {
        for (int i = 0; i < lenRoomSet; i++) {
            if (find_state[i] != FOUND_NOW) {
                continue;
            } // 只通过FOUND_NOW
            find_state[i] = HAD_FOUND;
            for (int gate = 0; gate < NUM_DIRECTIONS; gate++) {
                const int j = ROOM_GetRoomFromNet(i, gate);
                if (j == SET_ILLEGAL_INDEX) {
                    continue;
                } // 只通过有效索引
                if (find_state[j] == NOT_FOUND) {
                    TEMPO_RenewElem(roomSet[j].elem);
                    TEMPO_GetElemDstRect(roomSet[j].elem, &RDISet[j].dst_rect[1]);
                    ROOM_Renew_DstRect(RDISet[i].dst_rect[1], gate, &RDISet[j].dst_rect[1]);
                    find_state[j] = FOUND_NOW;
                    RDISet[j].depth = depth;
                }
                TEMPO_GetElemDstRect(roomSet[j].elem, &RDISet[j].dst_rects[gate][1]);
                ROOM_Renew_DstRect(RDISet[i].dst_rect[1], gate, &RDISet[j].dst_rects[gate][1]);
                RDISet[j].depths[gate] = depth;
            }
        } // 遍历find
    }
}


// DRAW ================================================================================================================
void ROOM_Draw_BDI() {
    for (int i = 0; i < lenRoomSet; i++) {
        for (int j = 0; j < NUM_DIRECTIONS; j++) {
            if (RDISet[i].depths[j] < DEPTH_ILLEGAL) {
                continue;
            }
            const SDL_FRect dst_rect = EASE_GetFRect(RDISet[i].dst_rects[j][0], RDISet[i].dst_rects[j][1], rate);
            TEMPO_SetElemDstRect(roomSet[i].elem, dst_rect);
            TEMPO_DrawElem(roomSet[i].elem);
        }
    }
    for (int i = 0; i < lenRoomSet; i++) {
        for (int j = 0; j < NUM_DIRECTIONS; j++) {
            if (RDISet[i].depths[j] < DEPTH_ILLEGAL) {
                continue;
            }
            const SDL_FRect dst_rect = EASE_GetFRect(RDISet[i].dst_rects[j][0], RDISet[i].dst_rects[j][1], rate);
            SDL_RenderTexture(mazeRenderer, cover[j], NULL, &dst_rect);
        }
    }
    for (int i = 0; i < lenRoomSet; i++) {
        if (RDISet[i].depth > DEPTH_ILLEGAL) {
            const SDL_FRect dst_rect = EASE_GetFRect(RDISet[i].dst_rect[0], RDISet[i].dst_rect[1], rate);
            TEMPO_SetElemDstRect(roomSet[i].elem, dst_rect);
            TEMPO_DrawElem(roomSet[i].elem);
        }
    }
}

