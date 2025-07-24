#include "block.h"


const SDL_FRect BLOCK_DEFAULT_GID_RECT = {0, 0, 16, 16};
bool SDL_CompareSDLColor(const SDL_Color x, const SDL_Color y) {
    return x.r == y.r && x.g == y.g && x.b == y.b && x.a == y.a;
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
static void BLOCK_SetNetCenter(const char* para);
SDL_FRect EASE_GetFRect(const SDL_FRect rect1, const SDL_FRect rect2, const float t) {
    SDL_FRect rect;
    rect.x = (1 - t) * rect1.x + t * rect2.x;
    rect.y = (1 - t) * rect1.y + t * rect2.y;
    rect.w = (1 - t) * rect1.w + t * rect2.w;
    rect.h = (1 - t) * rect1.h + t * rect2.h;
    return rect;
}


// BLOCK NET ===========================================================================================================
Direction** blockNet;
static int blockNetCenter = 0;


static void BLOCK_LoadBlockNet() {
    blockNet = (Direction**)allocate2DArray(lenBlockSet, lenBlockSet, sizeof(Direction));
    if (blockNet == NULL) {
        printf("%s.\n", __func__);
        return;
    } // Req Condition
    for (int i = 0; i < lenBlockSet; i++) {
        for (int j = 0; j < lenBlockSet; j++) {
            blockNet[i][j] = DIRECTION_ILLEGAL;
            for (int k = 0; k < NUM_DIRECTIONS; k++) {
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
                case DIRECTION_W: {
                    if (blockNet[j][i] != DIRECTION_S) {
                        blockNet[i][j] = DIRECTION_ILLEGAL;
                    }
                    break;
                }
                case DIRECTION_A: {
                    if (blockNet[j][i] != DIRECTION_D) {
                        blockNet[i][j] = DIRECTION_ILLEGAL;
                    }
                    break;
                }
                case DIRECTION_S: {
                    if (blockNet[j][i] != DIRECTION_W) {
                        blockNet[i][j] = DIRECTION_ILLEGAL;
                    }
                    break;
                }
                case DIRECTION_D: {
                    if (blockNet[j][i] != DIRECTION_A) {
                        blockNet[i][j] = DIRECTION_ILLEGAL;
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
static int BLOCK_GetBlockFromNet(const int i, const Direction gate) {
    for (int j = 0; j < lenBlockSet; j++) {
        if (blockNet[i][j] == gate) {
            return j;
        }
    }
    return i;
}


// BLOCK COVER =========================================================================================================
static SDL_Texture* cover[NUM_DIRECTIONS];
static void BLOCK_LoadBlockCover() {
    cover[DIRECTION_W] = IMG_LoadTexture(mazeRenderer, "../images/blocks/coverW.png");
    cover[DIRECTION_A] = IMG_LoadTexture(mazeRenderer, "../images/blocks/coverA.png");
    cover[DIRECTION_S] = IMG_LoadTexture(mazeRenderer, "../images/blocks/coverS.png");
    cover[DIRECTION_D] = IMG_LoadTexture(mazeRenderer, "../images/blocks/coverD.png");
    for (int i = 0; i < NUM_DIRECTIONS; i++) {
        SDL_SetTextureScaleMode(cover[i], SDL_SCALEMODE_NEAREST);
    }
}
static void BLOCK_UnloadBlockCover() {
    for (int i = 0; i < NUM_DIRECTIONS; i++) {
        SDL_DestroyTexture(cover[i]);
    }
}


// BLOCK ===============================================================================================================
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
    block->surface = surface;

    // load wall
    block->wall = (bool**)allocate2DArray(surface->w, surface->h, sizeof(bool)); // malloc
    for (int i = 0; i < surface->w; i++) {
        for (int j = 0; j < surface->h; j++) {
            block->wall[i][j] = true;
        }
    }
    for (int i = 1; i < surface->w-1; i++) {
        for (int j = 1; j < surface->h-1; j++) {
            SDL_Color color = {0};
            SDL_ReadSurfaceSDLColor(surface, i, j, &color);
            if (SDL_CompareSDLColor(color, (SDL_Color){255, 255, 255, 255}) == true) {
                block->wall[i][j] = false;
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
                    block->wall[i][j] = false;
                }
            }
        }
    }

    // load color
    SDL_ReadSurfaceSDLColor(surface, 0, 0, &block->color);

    // load elem
    char para[] = "00FF00FF";
    loadStringFromSDLColor(para, block->color);
    Elem* elem = TEMPO_CreateElem(
        ELEM_TYPE_FILE,
        path,
        40,
        &BLOCK_DEFAULT_GID_RECT,
        &BLOCK_SetNetCenter,
        para); // malloc
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
            block->gateColors[DIRECTION_W] = gateColor;
        }
    }
    // A
    for (int i = 0, j = 1; j < surface->h-1; j++) {
        SDL_Color gateColor = BLOCK_DEFAULT_COLOR;
        SDL_ReadSurfaceSDLColor(surface, i, j, &gateColor);
        if (SDL_CompareSDLColor(gateColor, BLOCK_DEFAULT_COLOR) == false
        && SDL_CompareSDLColor(gateColor, block->color) == false) {
            block->gateColors[DIRECTION_A] = gateColor;
        }
    }
    // S
    for (int i = 1, j = surface->h-1 ; i < surface->w-1; i++) {
        SDL_Color gateColor = BLOCK_DEFAULT_COLOR;
        SDL_ReadSurfaceSDLColor(surface, i, j, &gateColor);
        if (SDL_CompareSDLColor(gateColor, BLOCK_DEFAULT_COLOR) == false
        && SDL_CompareSDLColor(gateColor, block->color) == false) {
            block->gateColors[DIRECTION_S] = gateColor;
        }
    }
    // D
    for (int i = surface->w-1, j = 1; j < surface->h-1; j++) {
        SDL_Color gateColor = BLOCK_DEFAULT_COLOR;
        SDL_ReadSurfaceSDLColor(surface, i, j, &gateColor);
        if (SDL_CompareSDLColor(gateColor, BLOCK_DEFAULT_COLOR) == false
        && SDL_CompareSDLColor(gateColor, block->color) == false) {
            block->gateColors[DIRECTION_D] = gateColor;
        }
    }
    //
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
    if (block->surface != NULL) {
        SDL_DestroySurface(block->surface); // free
        block->surface = NULL;
    }
}


// BLOCK SET ===========================================================================================================
int lenBlockSet;
Block* blockSet;
const char* blockSetPath = "../config/maze/blockSet.toml";
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


// BLOCK DRAW INFO =====================================================================================================
static Uint64 time_start = 0, time_end = 0;
static Uint64 period = 1000;
typedef struct BlockDrawInfo {
    SDL_FRect dst_rect[2];
    int depth;
    SDL_FRect dst_rects[NUM_DIRECTIONS][2];
    int depths[NUM_DIRECTIONS];
} BDI;
BDI* BDISet;
float rate;
static void BLOCK_LoadBDISet() {
    BDISet = malloc(lenBlockSet * sizeof(BDI));
    if (BDISet == NULL) {
        printf("%s.\n", __func__);
        return;
    }
    for (int i = 0; i < lenBlockSet; i++) {
        BDISet[i] = (BDI){0};
    }
}
static void BLOCK_UnloadBDISet() {
    if (BDISet != NULL) {
        free(BDISet);
        BDISet = NULL;
    }
}


static void BLOCK_RenewBDISet() {
    if (SDL_GetTicks() > time_end) {
        for (int i = 0; i < lenBlockSet; i++) {
            BDISet[i].dst_rect[0] = BDISet[i].dst_rect[1];
            for (int j = 0; j < NUM_DIRECTIONS; j++) {
                BDISet[i].dst_rects[j][0] = BDISet[i].dst_rects[j][1];
            }
        }
        rate = 1;
        return;
    }
    rate = EASE_Sin2((float)(SDL_GetTicks() - time_start) / (float)period);
}


// TRIG ================================================================================================================
static void BLOCK_SetNetCenter(const char* para) {
    if (para == NULL) {
        return;
    }
    for (int i = 0; i < NUM_DIRECTIONS; i++) {
        char string[] = "0123456789";
        loadStringFromSDLColor(string, blockSet[i].color);
        if (blockNetCenter != i && strcmp(para, string) == 0) {
            blockNetCenter = i;
            time_start = SDL_GetTicks();
            time_end = time_start + period;
            return;
        }
    }
}


// LOAD ================================================================================================================
void BLOCK_Load() {
    BLOCK_LoadBlockCover();
    BLOCK_LoadBlockSet();
    BLOCK_LoadBlockNet();
    BLOCK_LoadBDISet();
}
void BLOCK_Unload() {
    BLOCK_UnloadBDISet();
    BLOCK_UnloadBlockCover();
    BLOCK_UnloadBlockNet();
    BLOCK_UnloadBlockSet();
}


// RENEW ===============================================================================================================
void BLOCK_Renew_DstRect(const SDL_FRect dst_rect1, const Direction gate, SDL_FRect* dst_rect2) {
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
void BLOCK_Renew_Block() {}
void BLOCK_Renew() {

    BLOCK_RenewBDISet();

    if (0 > blockNetCenter || blockNetCenter >= lenBlockSet) {
        printf("%s.\n", __func__);
        return;
    }
    bool need_renew = true;

    int find[lenBlockSet];
    enum {NOT_FOUND, FOUND_NOW, HAD_FOUND};
    for (int i = 0; i < lenBlockSet; i++) {
        find[i] = NOT_FOUND;
    }
    find[blockNetCenter] = FOUND_NOW;

    TEMPO_SetElemGidRect(blockSet[blockNetCenter].elem, BLOCK_DEFAULT_GID_RECT);
    TEMPO_RenewElem(blockSet[blockNetCenter].elem);
    // TEMPO_GetElemDstRect(blockSet[blockNetCenter].elem, &blockSet[blockNetCenter].dst_rect);
    TEMPO_GetElemDstRect(blockSet[blockNetCenter].elem, &BDISet[blockNetCenter].dst_rect[1]);

    int depth;
    for (int i = 0; i < lenBlockSet; i++) {
        BDISet[i].depth = 0;
        for (int j = 0; j < NUM_DIRECTIONS; j++) {
            BDISet[i].depths[j] = 0;
        }
    }
    BDISet[blockNetCenter].depth = depth = 1;

    while (need_renew) {
        depth++;
        for (int i = 0; i < lenBlockSet; i++) {
            if (find[i] != FOUND_NOW) {
                continue;
            }
            find[i] = HAD_FOUND;
            for (int j = 0; j < NUM_DIRECTIONS; j++) {
                const int idx = BLOCK_GetBlockFromNet(i, j);
                if (idx == i) {
                    continue;
                }
                switch (find[idx]) {
                    case NOT_FOUND: {
                        // renew dst, depth by blockSet[i], j
                        TEMPO_RenewElem(blockSet[idx].elem);
                        TEMPO_GetElemDstRect(blockSet[idx].elem, &BDISet[idx].dst_rect[1]);
                        BLOCK_Renew_DstRect(BDISet[i].dst_rect[1], j, &BDISet[idx].dst_rect[1]);
                        find[idx] = FOUND_NOW;
                        BDISet[idx].depth = depth;
                        break;
                    }
                    case FOUND_NOW:
                    case HAD_FOUND: {
                        // renew gate.dst, gate.depth by blockSet[i], j
                        break;
                    }
                    default: break;
                }
                TEMPO_GetElemDstRect(blockSet[idx].elem, &BDISet[idx].dst_rects[j][1]);
                BLOCK_Renew_DstRect(BDISet[i].dst_rect[1], j, &BDISet[idx].dst_rects[j][1]);
                BDISet[idx].depths[j] = depth;
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


// DRAW ================================================================================================================
void BLOCK_Draw_BDI() {
    for (int i = 0; i < lenBlockSet; i++) {
        for (int j = 0; j < NUM_DIRECTIONS; j++) {
            if (BDISet[i].depths[j] < 0) {
                continue;
            }
            const SDL_FRect dst_rect = EASE_GetFRect(BDISet[i].dst_rects[j][0], BDISet[i].dst_rects[j][1], rate);
            TEMPO_SetElemDstRect(blockSet[i].elem, dst_rect);
            TEMPO_DrawElem(blockSet[i].elem);
        }
    }
    for (int i = 0; i < lenBlockSet; i++) {
        for (int j = 0; j < NUM_DIRECTIONS; j++) {
            if (BDISet[i].depths[j] < 0) {
                continue;
            }
            const SDL_FRect dst_rect = EASE_GetFRect(BDISet[i].dst_rects[j][0], BDISet[i].dst_rects[j][1], rate);
            SDL_RenderTexture(mazeRenderer, cover[j], NULL, &dst_rect);
        }
    }
    for (int i = 0; i < lenBlockSet; i++) {
        if (BDISet[i].depth > 0) {
            const SDL_FRect dst_rect = EASE_GetFRect(BDISet[i].dst_rect[0], BDISet[i].dst_rect[1], rate);
            TEMPO_SetElemDstRect(blockSet[i].elem, dst_rect);
            TEMPO_DrawElem(blockSet[i].elem);
        }
    }
}

