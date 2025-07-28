#include "zero.h"


// RDI: ROOM DRAW INFO =================================================================================================
static Uint64 time_start = 0, time_end = 0, period = 1000;
typedef struct RoomDrawInfo {
    Elem* elem;
    SDL_FRect dst_rect[2];
    int depth;
    SDL_FRect dst_rects[NUM_DIRECTIONS][2];
    int depths[NUM_DIRECTIONS];
} RoomDrawInfo;
RoomDrawInfo* RDI;
float rate;
static const int DEPTH_ILLEGAL = -1;
static const SDL_FRect ROOM_DEFAULT_GID_RECT = {0, 0, 16, 16};
static int roomCenter = 0;
static void ROOM_SetNetCenter(const char* para);
static int ROOM_GetRoomFromNet(const int i, const Direction gate) {
    for (int j = 0; j < lenRoomSet; j++) {
        if (roomNet[i][j] == gate) {
            return j;
        }
    }
    return SET_ILLEGAL_INDEX;
}


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

void ROOM_LoadRDISet() {
    ROOM_LoadRoomCover();
    RDI = malloc(lenRoomSet * sizeof(RoomDrawInfo));
    if (RDI == NULL) {
        printf("%s.\n", __func__);
        return;
    }
    for (int i = 0; i < lenRoomSet; i++) {
        RDI[i] = (RoomDrawInfo){0};
        const Room* room = &roomSet[i];
        const ElemInfo info = {
            ELEM_TYPE_FILE, room->path,
            40, &ROOM_DEFAULT_GID_RECT,
            &ROOM_SetNetCenter, SDL_GetStringFromSDLColor(room->color)
        };
        Elem* elem = TEMPO_CreateElem(info); // malloc
        if (elem == NULL) {
            printf("%s: failed to create elem.\n", __func__);
            return;
        }
        RDI[i].elem = elem;
    }
}
void ROOM_UnloadRDISet() {
    ROOM_UnloadRoomCover();
    if (RDI != NULL) {
        for (int i = 0; i < lenRoomSet; i++) {
            if (RDI[i].elem != NULL) {
                TEMPO_DestroyElem(RDI[i].elem);
            }
        }
        free(RDI);
        RDI = NULL;
    }
}
static void ROOM_Renew_RDISet_rate() {
    if (SDL_GetTicks() > time_end) {
        for (int i = 0; i < lenRoomSet; i++) {
            RDI[i].dst_rect[0] = RDI[i].dst_rect[1];
            for (int j = 0; j < NUM_DIRECTIONS; j++) {
                RDI[i].dst_rects[j][0] = RDI[i].dst_rects[j][1];
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


void RDI_Renew_DstRect(const SDL_FRect dst_rect1, const Direction gate, SDL_FRect* dst_rect2) {
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
void RDI_Renew_depth() {
    for (int i = 0; i < lenRoomSet; i++) {
        RDI[i].depth = 0;
    }
    RDI[roomCenter].depth = 1;
}
void RDI_Renew() {
    ROOM_Renew_RDISet_rate();

    if (0 > roomCenter || roomCenter >= lenRoomSet) {
        printf("%s.\n", __func__);
        return;
    }
    // init
    enum {NOT_FOUND, FOUND_NOW, HAD_FOUND};
    int find_state[lenRoomSet];
    for (int i = 0; i < lenRoomSet; i++) {
        find_state[i] = NOT_FOUND;
    }
    find_state[roomCenter] = FOUND_NOW;

    for (int i = 0; i < lenRoomSet; i++) {
        RDI[i].depth = DEPTH_ILLEGAL;
        for (int j = 0; j < NUM_DIRECTIONS; j++) {
            RDI[i].depths[j] = DEPTH_ILLEGAL;
        }
    }
    RDI[roomCenter].depth = 0;
;

    TEMPO_SetElemGidRect(RDI[roomCenter].elem, ROOM_DEFAULT_GID_RECT);
    TEMPO_RenewElem(RDI[roomCenter].elem);
    TEMPO_GetElemDstRect(RDI[roomCenter].elem, &RDI[roomCenter].dst_rect[1]);


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
                    TEMPO_RenewElem(RDI[j].elem);
                    TEMPO_GetElemDstRect(RDI[j].elem, &RDI[j].dst_rect[1]);
                    RDI_Renew_DstRect(RDI[i].dst_rect[1], gate, &RDI[j].dst_rect[1]);
                    find_state[j] = FOUND_NOW;
                    RDI[j].depth = depth;
                }
                TEMPO_GetElemDstRect(RDI[j].elem, &RDI[j].dst_rects[gate][1]);
                RDI_Renew_DstRect(RDI[i].dst_rect[1], gate, &RDI[j].dst_rects[gate][1]);
                RDI[j].depths[gate] = depth;
            }
        } // 遍历find
    }

}


static void ROOM_SetNetCenter(const char* para) {
    if (para == NULL) {
        return;
    }
    for (int i = 0; i < NUM_DIRECTIONS; i++) {
        if (roomCenter != i && strcmp(para, SDL_GetStringFromSDLColor(roomSet[i].color)) == 0) {
            roomCenter = i;
            time_start = SDL_GetTicks();
            time_end = time_start + period;
            return;
        }
    }
}



void RDI_Draw() {
    for (int i = 0; i < lenRoomSet; i++) {
        for (int j = 0; j < NUM_DIRECTIONS; j++) {
            if (RDI[i].depths[j] < DEPTH_ILLEGAL) {
                continue;
            }
            const SDL_FRect dst_rect = EASE_GetFRect(RDI[i].dst_rects[j][0], RDI[i].dst_rects[j][1], rate);
            TEMPO_SetElemDstRect(RDI[i].elem, dst_rect);
            TEMPO_DrawElem(RDI[i].elem);
        }
    }
    for (int i = 0; i < lenRoomSet; i++) {
        for (int j = 0; j < NUM_DIRECTIONS; j++) {
            if (RDI[i].depths[j] < DEPTH_ILLEGAL) {
                continue;
            }
            const SDL_FRect dst_rect = EASE_GetFRect(RDI[i].dst_rects[j][0], RDI[i].dst_rects[j][1], rate);
            SDL_RenderTexture(mazeRenderer, cover[j], NULL, &dst_rect);
        }
    }
    for (int i = 0; i < lenRoomSet; i++) {
        if (RDI[i].depth > DEPTH_ILLEGAL) {
            const SDL_FRect dst_rect = EASE_GetFRect(RDI[i].dst_rect[0], RDI[i].dst_rect[1], rate);
            TEMPO_SetElemDstRect(RDI[i].elem, dst_rect);
            TEMPO_DrawElem(RDI[i].elem);
        }
    }
}




// ZERO ================================================================================================================
void ZERO_Load() {
    ROOM_LoadRDISet();
}
void ZERO_Unload() {
    ROOM_UnloadRDISet();
}
void ZERO_Renew() {
    RDI_Renew();
}
void ZERO_Draw() {
    RDI_Draw();
}

