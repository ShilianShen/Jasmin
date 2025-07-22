#include "block.h"


int lenBlockSet;
Block* blockSet;
BlockGate** blockNet;
const char* blockSetPath = "../src/maze/block/blockSet.toml";
static SDL_Texture* cover[NUM_BLOCK_GATES];
static int blockNetCenter = 0;

const SDL_FRect BLOCK_DEFAULT_GID_RECT = {0, 0, 16, 16};
char* SDL_GetStringFromSDLColor(const SDL_Color color) {
    static char string[32];
    snprintf(string, 31, "[%d, %d, %d, %d]", color.r, color.g, color.b, color.a);
    return string;
}
char* SDL_GetStringFromFRect(const SDL_FRect rect) {
    static char string[32];
    snprintf(string, 31, "[%.2f, %.2f, %.2f, %.2f]", rect.x, rect.y, rect.w, rect.h);
    return string;
}


const SDL_Color BLOCK_DEFAULT_COLOR = {0, 0, 0, 0};
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


bool SDL_ReadSurfaceSDLColor(SDL_Surface* surface, const int x, const int y, SDL_Color* color) {
    if (surface == NULL) {
        printf("%s: surface is null.\n", __func__);
        return false;
    }
    SDL_ReadSurfacePixel(surface, x, y, &color->r, &color->g, &color->b, &color->a);
    return true;
}
bool SDL_CompareSDLColor(const SDL_Color color1, const SDL_Color color2) {
    return
    color1.r == color2.r
    && color1.g == color2.g
    && color1.b == color2.b
    && color1.a == color2.a;
}


void BLOCK_SetNetCenter(const char* para) {
    if (para == NULL) {
        return;
    }
    printf("1\n");
    if (strlen(para) < 4) {
        return;
    } // Req Condition
    printf("2\n");
    for (int i = 0; i < lenBlockSet; i++) {
        if ((void*)para == (void*)&blockSet[i]) {
            blockNetCenter = i;
        }
    }
    printf("3\n");
}


static void BLOCK_LoadBlock(Block* block, const char* path) {
    // Req Condition
    if (block == NULL) {
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
    *block = (Block){0};

    // load w, h
    block->w = surface->w;
    block->h = surface->h;

    // load wall
    block->wall = (int**)allocate2DArray(surface->w, surface->h, sizeof(int)); // malloc

    // load color
    SDL_ReadSurfaceSDLColor(surface, 0, 0, &block->color);

    // load elem
    Elem* elem = TEMPO_CreateElem(
        ELEM_TYPE_FILE,
        path,
        40,
        &BLOCK_DEFAULT_GID_RECT,
        BLOCK_SetNetCenter,
        (const char*)block); // malloc
    if (elem == NULL) {
        printf("%s: failed to create elem.\n", __func__);
        return;
    }
    block->elem = elem;

    // load gate
    // W
    for (int i = 1, j = 0; i < surface->w-1; i++) {
        SDL_Color gateColor = BLOCK_DEFAULT_COLOR;
        SDL_ReadSurfaceSDLColor(surface, i, j, &gateColor);
        if (SDL_CompareSDLColor(gateColor, BLOCK_DEFAULT_COLOR) == false
        && SDL_CompareSDLColor(gateColor, block->color) == false) {
            block->gateColors[BLOCK_GATE_W] = gateColor;
        }
    }
    // A
    for (int i = 0, j = 1; j < surface->h-1; j++) {
        SDL_Color gateColor = BLOCK_DEFAULT_COLOR;
        SDL_ReadSurfaceSDLColor(surface, i, j, &gateColor);
        if (SDL_CompareSDLColor(gateColor, BLOCK_DEFAULT_COLOR) == false
        && SDL_CompareSDLColor(gateColor, block->color) == false) {
            block->gateColors[BLOCK_GATE_A] = gateColor;
        }
    }
    // S
    for (int i = 1, j = surface->h-1 ; i < surface->w-1; i++) {
        SDL_Color gateColor = BLOCK_DEFAULT_COLOR;
        SDL_ReadSurfaceSDLColor(surface, i, j, &gateColor);
        if (SDL_CompareSDLColor(gateColor, BLOCK_DEFAULT_COLOR) == false
        && SDL_CompareSDLColor(gateColor, block->color) == false) {
            block->gateColors[BLOCK_GATE_S] = gateColor;
        }
    }
    // D
    for (int i = surface->w-1, j = 1; j < surface->h-1; j++) {
        SDL_Color gateColor = BLOCK_DEFAULT_COLOR;
        SDL_ReadSurfaceSDLColor(surface, i, j, &gateColor);
        if (SDL_CompareSDLColor(gateColor, BLOCK_DEFAULT_COLOR) == false
        && SDL_CompareSDLColor(gateColor, block->color) == false) {
            block->gateColors[BLOCK_GATE_D] = gateColor;
        }
    }
    //
    SDL_DestroySurface(surface); // free
}
static void BLOCK_UnloadBlock(Block* block) {
    if (block == NULL) {
        return;
    }
    if (block->wall != NULL) {
        free2DArray((void**)block->wall, block->w);
        block->wall = NULL;
    }
    if (block->elem != NULL) {
        TEMPO_DestroyElem(block->elem);
        block->elem = NULL;
    }
}


static void BLOCK_LoadBlockSet() {
    // Req Condition
    if (blockSetPath == NULL) {
        printf("%s.\n", __func__);
        return;
    }

    toml_table_t* tomlFile = getToml(blockSetPath); // malloc
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
    lenBlockSet = toml_array_nelem(tomlBlocks);
    blockSet = malloc(lenBlockSet * sizeof(Block)); // malloc
    for (int i = 0; i < lenBlockSet; i++) {
        // Req Condition
        const toml_datum_t tomlBlockPath = toml_string_at(tomlBlocks, i);
        if (tomlBlockPath.ok == false) {
            printf("%s.\n", __func__);
            continue;
        }
        //
        BLOCK_LoadBlock(&blockSet[i], tomlBlockPath.u.s); // malloc
    }
    toml_free(tomlFile); // free
}
static void BLOCK_UnloadBlockSet() {
    if (blockSet != NULL) {
        for (int i = 0; i < lenBlockSet; i++) {
            BLOCK_UnloadBlock(&blockSet[i]); // free
        }
        free(blockSet); // free
        blockSet = NULL;
        lenBlockSet = 0;
    }
}


static void BLOCK_LoadBlockNet() {
    blockNet = (BlockGate**)allocate2DArray(lenBlockSet, lenBlockSet, sizeof(BlockGate));
    if (blockNet == NULL) {
        printf("%s.\n", __func__);
        return;
    } // Req Condition
    for (int i = 0; i < lenBlockSet; i++) {
        for (int j = 0; j < lenBlockSet; j++) {
            blockNet[i][j] = BLOCK_GATE_ILLEGAL;
            for (int k = 0; k < NUM_BLOCK_GATES; k++) {
                const SDL_Color color1 = blockSet[i].gateColors[k];
                const SDL_Color color2 = blockSet[j].color;
                if (SDL_CompareSDLColor(color1, color2) == true) {
                    blockNet[i][j] = k;
                    break;
                }
            }
        }
    }
    for (int i = 0; i < lenBlockSet; i++) {
        for (int j = 0; j < lenBlockSet; j++) {
            switch (blockNet[i][j]) {
                case BLOCK_GATE_W: {
                    if (blockNet[j][i] != BLOCK_GATE_S) {
                        blockNet[i][j] = BLOCK_GATE_ILLEGAL;
                    }
                    break;
                }
                case BLOCK_GATE_A: {
                    if (blockNet[j][i] != BLOCK_GATE_D) {
                        blockNet[i][j] = BLOCK_GATE_ILLEGAL;
                    }
                    break;
                }
                case BLOCK_GATE_S: {
                    if (blockNet[j][i] != BLOCK_GATE_W) {
                        blockNet[i][j] = BLOCK_GATE_ILLEGAL;
                    }
                    break;
                }
                case BLOCK_GATE_D: {
                    if (blockNet[j][i] != BLOCK_GATE_A) {
                        blockNet[i][j] = BLOCK_GATE_ILLEGAL;
                    }
                    break;
                }
                default: break;
            }
        }
    }
}
static void BLOCK_UnloadBlockNet() {
    if (blockNet != NULL) {
        free2DArray((void**)blockNet, lenBlockSet);
        blockNet = NULL;
    }
}
static int BLOCK_GetBlockFromNet(const int i, const BlockGate gate) {
    for (int j = 0; j < lenBlockSet; j++) {
        if (blockNet[i][j] == gate) {
            return j;
        }
    }
    return i;
}


void BLOCK_Load() {
    cover[BLOCK_GATE_W] = IMG_LoadTexture(mazeRenderer, "../images/blocks/coverW.png");
    cover[BLOCK_GATE_A] = IMG_LoadTexture(mazeRenderer, "../images/blocks/coverA.png");
    cover[BLOCK_GATE_S] = IMG_LoadTexture(mazeRenderer, "../images/blocks/coverS.png");
    cover[BLOCK_GATE_D] = IMG_LoadTexture(mazeRenderer, "../images/blocks/coverD.png");
    BLOCK_LoadBlockSet();
    BLOCK_LoadBlockNet();
}
void BLOCK_Unload() {
    BLOCK_UnloadBlockNet();
    BLOCK_UnloadBlockSet();
    for (int i = 0; i < NUM_BLOCK_GATES; i++) {
        SDL_DestroyTexture(cover[i]);
    }
}


void BLOCK_RenewDstRect(const SDL_FRect dst_rect1, const BlockGate gate, SDL_FRect* dst_rect2) {
    if (dst_rect2 == NULL) {
        printf("%s.\n", __func__);
        return;
    }
    float dx = 0, dy = 0;
    switch (gate) {
        case BLOCK_GATE_W: {
            dy = -dst_rect2->h;
            break;
        }
        case BLOCK_GATE_A: {
            dx = -dst_rect2->w;
            break;
        }
        case BLOCK_GATE_S: {
            dy = dst_rect1.h;
            break;
        }
        case BLOCK_GATE_D: {
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
void BLOCK_Renew() {
    if (0 > blockNetCenter || blockNetCenter >= lenBlockSet) {
        printf("%s.\n", __func__);
        return;
    }
    int find[lenBlockSet];
    bool need_renew = true;
    int depth = 0;
    enum {NOT_FOUND, FOUND_NOW, HAD_FOUND};
    for (int i = 0; i < lenBlockSet; i++) {
        find[i] = NOT_FOUND;
    }
    find[blockNetCenter] = FOUND_NOW;
    DEBUG_SendMessageR("center: %d\n", blockNetCenter);
    TEMPO_SetElemGidRect(blockSet[blockNetCenter].elem, BLOCK_DEFAULT_GID_RECT);
    TEMPO_RenewElem(blockSet[blockNetCenter].elem);
    TEMPO_GetElemDstRect(blockSet[blockNetCenter].elem, &blockSet[blockNetCenter].dst_rect);
    for (int i = 0; i < lenBlockSet; i++) {
        blockSet[i].depth = 0;
        for (int j = 0; j < NUM_BLOCK_GATES; j++) {
            blockSet[i].gateDepths[j] = 0;
        }
    }
    while (need_renew) {
        depth++;
        for (int i = 0; i < lenBlockSet; i++) {
            if (find[i] != FOUND_NOW) {
                continue;
            }
            find[i] = HAD_FOUND;
            for (int j = 0; j < NUM_BLOCK_GATES; j++) {
                const int idx = BLOCK_GetBlockFromNet(i, j);
                if (idx == i) {
                    continue;
                }
                switch (find[idx]) {
                    case NOT_FOUND: {
                        // renew dst, depth by blockSet[i], j
                        TEMPO_RenewElem(blockSet[idx].elem);
                        TEMPO_GetElemDstRect(blockSet[idx].elem, &blockSet[idx].dst_rect);
                        BLOCK_RenewDstRect(blockSet[i].dst_rect, j, &blockSet[idx].dst_rect);
                        blockSet[idx].depth = depth;
                        find[idx] = FOUND_NOW;
                        break;
                    }
                    case HAD_FOUND: {
                        // renew gate.dst, gate.depth by blockSet[i], j
                        TEMPO_GetElemDstRect(blockSet[idx].elem, &blockSet[idx].dst_rects[j]);
                        BLOCK_RenewDstRect(blockSet[i].dst_rect, j, &blockSet[idx].dst_rects[j]);
                        blockSet[idx].gateDepths[j] = depth;
                        break;
                    }
                    default: break;
                }
            }
        } // 遍历find
        need_renew = false;
        for (int i = 0; i < lenBlockSet; i++) {
            if (find[i] == FOUND_NOW) {
                need_renew = true;
                break;
            }
        }
    }
}
void BLOCK_Draw() {
    int max_depth = 0;
    for (int i = 0; i < lenBlockSet; i++) {
        if (blockSet[i].depth > max_depth) {
            max_depth = blockSet[i].depth;
        }
        for (int j = 0; j < NUM_BLOCK_GATES; j++) {
            if (blockSet[i].gateDepths[j] > max_depth) {
                max_depth = blockSet[i].gateDepths[j];
            }
        }
    }
    for (int z = max_depth; z > 0; z--) {
        for (int i = 0; i < lenBlockSet; i++) {
            if (blockSet[i].depth == z) {
                DEBUG_SendMessageR("%d\n", i);
                TEMPO_SetElemDstRect(blockSet[i].elem, blockSet[i].dst_rect);
                TEMPO_DrawElem(blockSet[i].elem);
            }
            for (int j = 0; j < NUM_BLOCK_GATES; j++) {
                if (blockSet[i].gateDepths[j] == z) {
                    DEBUG_SendMessageR("%d, %d\n", i, j);
                    TEMPO_SetElemDstRect(blockSet[i].elem, blockSet[i].dst_rects[j]);
                    TEMPO_DrawElem(blockSet[i].elem);
                    SDL_RenderTexture(mazeRenderer, cover[j], NULL, &blockSet[i].dst_rects[j]);
                }
            }
        }
    }

}