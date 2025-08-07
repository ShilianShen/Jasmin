#include "RDI.h"


// RDI: ROOM DRAW INFO =================================================================================================
static Uint64 time_start = 0, period = 1000;
static float rate;
static const int DEPTH_ILLEGAL = -1;
static const SDL_FRect DEFAULT_GID_RECT = {0, 0, 16, 16};
static int center = 0;


// FIND STATE ==========================================================================================================
enum FindState {NOT_FOUND, FOUND_NOW, HAD_FOUND};


// COVER ===============================================================================================================
static SDL_Texture* covers[NUM_DIRECTIONS];
static void ZERO_LoadRDI_Covers() {
    covers[DIRECTION_W] = IMG_LoadTexture(mazeRenderer, "../images/blocks/coverW.png");
    covers[DIRECTION_A] = IMG_LoadTexture(mazeRenderer, "../images/blocks/coverA.png");
    covers[DIRECTION_S] = IMG_LoadTexture(mazeRenderer, "../images/blocks/coverS.png");
    covers[DIRECTION_D] = IMG_LoadTexture(mazeRenderer, "../images/blocks/coverD.png");
    for (int i = 0; i < NUM_DIRECTIONS; i++) {
        SDL_SetTextureScaleMode(covers[i], SDL_SCALEMODE_NEAREST);
    }
}
static void ZERO_UnloadRDI_Covers() {
    for (int i = 0; i < NUM_DIRECTIONS; i++) {
        SDL_DestroyTexture(covers[i]);
    }
}


// FRAME ===============================================================================================================
enum Frame {BEFORE, AFTER, NOW, NUM_FRAMES};


// RDI =================================================================================================================
typedef struct RoomDrawInfo {
    Elem* elem;
    SDL_FRect dst_rect[NUM_FRAMES];
    int depth;
    SDL_FRect dst_rects[NUM_DIRECTIONS][NUM_FRAMES];
    int depths[NUM_DIRECTIONS];
} RDI;
static RDI* RDIs;
static void swap() {
    for (int i = 0; i < lenRoomSet; i++) {
        RDIs[i].dst_rect[BEFORE] = RDIs[i].dst_rect[NOW];
        for (int j = 0; j < NUM_DIRECTIONS; j++) {
            RDIs[i].dst_rects[j][BEFORE] = RDIs[i].dst_rects[j][NOW];
        }
    }
}
static void ZERO_SetRDICenter(const char* para) {
    if (para == NULL) {
        return;
    }
    for (int i = 0; i < NUM_DIRECTIONS; i++) {
        if (center != i && strcmp(para, SDL_GetStringFromSDLColor(roomSet[i].color)) == 0) {
            center = i;
            time_start = SDL_GetTicks();
            swap();
            return;
        }
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


// LOAD & UNLOAD =======================================================================================================
void ZERO_LoadRDI() {
    ZERO_LoadRDI_Covers();
    RDIs = malloc(lenRoomSet * sizeof(RDI));
    if (RDIs == NULL) {
        printf("%s.\n", __func__);
        return;
    }
    for (int i = 0; i < lenRoomSet; i++) {
        RDIs[i] = (RDI){0};
        const Room* room = &roomSet[i];
        const ElemInfo info = {
            ELEM_TYPE_FILE, room->path,
            40, &DEFAULT_GID_RECT,
            &ZERO_SetRDICenter, SDL_GetStringFromSDLColor(room->color)
        };
        Elem* elem = TEMPO_CreateElem(info); // malloc
        if (elem == NULL) {
            printf("%s: failed to create elem.\n", __func__);
            return;
        }
        RDIs[i].elem = elem;
    }
}
void ZERO_UnloadRDI() {
    ZERO_UnloadRDI_Covers();
    if (RDIs != NULL) {
        for (int i = 0; i < lenRoomSet; i++) {
            if (RDIs[i].elem != NULL) {
                TEMPO_DeleteElem(RDIs[i].elem);
            }
        }
        free(RDIs);
        RDIs = NULL;
    }
}


// RENEW ===============================================================================================================
static void ZERO_RenewRDI_Rate() {
    if (SDL_GetTicks() > time_start + period) {
        for (int i = 0; i < lenRoomSet; i++) {
            RDIs[i].dst_rect[BEFORE] = RDIs[i].dst_rect[AFTER];
            for (int j = 0; j < NUM_DIRECTIONS; j++) {
                RDIs[i].dst_rects[j][BEFORE] = RDIs[i].dst_rects[j][AFTER];
            }
        }
        rate = 1;
        return;
    }
    if (period == 0) {
        printf("%s.\n", __func__);
    }
    rate = EASE_Sin2((float)(SDL_GetTicks() - time_start) / (float)period);
}
static void ZERO_RenewRDI_DstRect(const SDL_FRect dst_rect1, const Direction gate, SDL_FRect* dst_rect2) {
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
static void ZERO_RenewRDI_Other() {
    if (0 > center || center >= lenRoomSet) {
        printf("%s.\n", __func__);
        return;
    }
    // init
    int find_state[lenRoomSet];
    for (int i = 0; i < lenRoomSet; i++) {
        find_state[i] = NOT_FOUND;
    }
    find_state[center] = FOUND_NOW;

    for (int i = 0; i < lenRoomSet; i++) {
        RDIs[i].depth = DEPTH_ILLEGAL;
        for (int j = 0; j < NUM_DIRECTIONS; j++) {
            RDIs[i].depths[j] = DEPTH_ILLEGAL;
        }
    }
    RDIs[center].depth = 0;
    ;

    TEMPO_SetElemGidRect(RDIs[center].elem, DEFAULT_GID_RECT);
    TEMPO_RenewElem(RDIs[center].elem);
    TEMPO_GetElemDstRect(RDIs[center].elem, &RDIs[center].dst_rect[AFTER]);


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
                    TEMPO_RenewElem(RDIs[j].elem);
                    TEMPO_GetElemDstRect(RDIs[j].elem, &RDIs[j].dst_rect[AFTER]);
                    ZERO_RenewRDI_DstRect(RDIs[i].dst_rect[AFTER], gate, &RDIs[j].dst_rect[AFTER]);
                    find_state[j] = FOUND_NOW;
                    RDIs[j].depth = depth;
                }
                TEMPO_GetElemDstRect(RDIs[j].elem, &RDIs[j].dst_rects[gate][AFTER]);
                ZERO_RenewRDI_DstRect(RDIs[i].dst_rect[AFTER], gate, &RDIs[j].dst_rects[gate][AFTER]);
                RDIs[j].depths[gate] = depth;
            }
        } // 遍历find
    }
}
void ZERO_RenewRDI() {
    ZERO_RenewRDI_Other();
    ZERO_RenewRDI_Rate();
    for (int i = 0; i < lenRoomSet; i++) {
        RDIs[i].dst_rect[NOW] = EASE_GetFRect(RDIs[i].dst_rect[BEFORE], RDIs[i].dst_rect[AFTER], rate);
        for (int j = 0; j < NUM_DIRECTIONS; j++) {
            RDIs[i].dst_rects[j][NOW] = EASE_GetFRect(RDIs[i].dst_rects[j][BEFORE], RDIs[i].dst_rects[j][AFTER], rate);
        }
    }
}


// DRAW ================================================================================================================
void ZERO_DrawRDI() {
    for (int i = 0; i < lenRoomSet; i++) {
        for (int j = 0; j < NUM_DIRECTIONS; j++) {
            if (RDIs[i].depths[j] < DEPTH_ILLEGAL) {
                continue;
            }
            TEMPO_SetElemDstRect(RDIs[i].elem, RDIs[i].dst_rects[j][NOW]);
            TEMPO_DrawElem(RDIs[i].elem);
        }
    }
    for (int i = 0; i < lenRoomSet; i++) {
        for (int j = 0; j < NUM_DIRECTIONS; j++) {
            if (RDIs[i].depths[j] < DEPTH_ILLEGAL) {
                continue;
            }
            SDL_RenderTexture(mazeRenderer, covers[j], NULL, &RDIs[i].dst_rects[j][NOW]);
        }
    }
    for (int i = 0; i < lenRoomSet; i++) {
        if (RDIs[i].depth > DEPTH_ILLEGAL) {
            TEMPO_SetElemDstRect(RDIs[i].elem, RDIs[i].dst_rect[NOW]);
            TEMPO_DrawElem(RDIs[i].elem);
        }
    }
}