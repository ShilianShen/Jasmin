#include "room.h"


const SDL_Color ROOM_DEFAULT_COLOR = {0, 0, 0, 0};



// ROOM NET ============================================================================================================
Direction** roomNet;
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

    // load path
    room->path = strdup(path);
    if (room->path == NULL) {
        printf("%s: failed to allocate room path.\n", __func__);
        return;
    }

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
    if (room->path != NULL) {
        free(room->path);
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


// LOAD ================================================================================================================
void ROOM_Load() {
    ROOM_LoadRoomSet();
    ROOM_LoadRoomNet();
}
void ROOM_Unload() {
    ROOM_UnloadRoomNet();
    ROOM_UnloadRoomSet();
}

void ROOM_Renew() {

}


