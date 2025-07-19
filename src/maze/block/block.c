#include "block.h"


int lenBlockSet;
Block* blockSet;


static int** getArray2D(const int w, int const h) {
    // 分配行指针 + 所有数据空间
    int **array = malloc(w * sizeof(int *) + w * h * sizeof(int));
    if (array == NULL) {
        perror("内存分配失败");
        exit(EXIT_FAILURE);
    }

    // 设置行指针指向数据区域
    int *data = (int *)(array + w);
    for (int i = 0; i < w; i++) {
        array[i] = data + i * h;
    }
    return array;
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
    block->wall = getArray2D(surface->w, surface->h); // malloc

    // load color
    SDL_ReadSurfacePixel(surface, 0, 0, &block->color.r, &block->color.g,  &block->color.b, &block->color.a);

    // load texture
    SDL_Texture* texture = IMG_LoadTexture(mazeRenderer, path);
    if (texture == NULL) {
        printf("%s: failed to load texture.\n", __func__);
    }
    block->texture = texture;
    //
    SDL_DestroySurface(surface); // free
}
static void BLOCK_UnloadBlock(Block* block) {
    if (block == NULL) {
        return;
    }
    if (block->wall != NULL) {
        free(block->wall);
        block->wall = NULL;
    }
    if (block->texture != NULL) {
        SDL_DestroyTexture(block->texture);
        block->texture = NULL;
    }
}


static void BLOCK_LoadBlockSet(const char* path) {
    // Req Condition
    if (path == NULL) {
        printf("%s.\n", __func__);
        return;
    }

    toml_table_t* tomlFile = getToml(path); // malloc
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


void BLOCK_Load() {
    const char* path = "/Users/shilianshen/Documents/Oolong Press/Lo-Tri/src/maze/block/blockSet.toml";
    BLOCK_LoadBlockSet(path);
}
void BLOCK_Unload() {
    BLOCK_UnloadBlockSet();
}


void BLOCK_Draw() {
    Block block = blockSet[0];
    SDL_FRect gid_rect = {0, 0, 10.f, 10.f};
    SDL_FRect dst_rect = {100, 100, 100, 100};
    SDL_LoadDstRectAligned(&dst_rect, block.texture, NULL, &gid_rect, NULL, 40);
    SDL_RenderTexture(mazeRenderer, block.texture, NULL, &dst_rect);
    DEBUG_SendMessageR("%d, %d, %.2f, %.2f, %.2f, %.2f\n", block.w, block.h, dst_rect.x, dst_rect.y, dst_rect.w, dst_rect.h);
}