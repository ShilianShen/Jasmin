#include "monster.h"


Monster* monsterSet;
int lenMonsterSet;


static void MONSTER_Load_Monster(Monster* monster, Room* block, const int x, const int y, Gene* gene) {
    if (monster == NULL) {
        printf("%s, f.\n", __func__);
        return;
    } // Req Condition
    monster->gene = gene;
    monster->posBirth.block = block;
    monster->posBirth.x = x;
    monster->posBirth.y = y;
}
static void MONSTER_Unload_Monster(Monster* monster) {
}

static void MONSTER_Load_MonsterSet() {
    //
    for (int i = 0; i < lenRoomSet; i++) {
        const Room* block = &roomSet[i];
        for (int x = 2; x < block->w-2; x++) {
            for (int y = 2; y < block->h-2; y++) {
                SDL_Color color;
                SDL_ReadSurfaceSDLColor(block->surface, x, y, &color);
                const Gene* gene = GENE_FindGeneFromColor(color);
                if (gene == NULL) {
                    continue;
                }
                lenMonsterSet++;
            }
        }
    }
    if (lenMonsterSet == 0) {
        printf("%s, f.\n", __func__);
        return;
    }
    monsterSet = malloc(lenMonsterSet * sizeof(Monster)); // malloc
    if (monsterSet == NULL) {
        printf("%s, f.\n", __func__);
        return;
    } // Req Condition
    //
    int idx = 0;
    for (int i = 0; i < lenRoomSet; i++) {
        Room* block = &roomSet[i];
        for (int x = 2; x < block->w-2; x++) {
            for (int y = 2; y < block->h-2; y++) {
                SDL_Color color;
                if (SDL_ReadSurfaceSDLColor(block->surface, x, y, &color) == false) {
                    printf("%s, f.\n", __func__);
                    continue;
                }
                Gene* gene = GENE_FindGeneFromColor(color);
                if (gene == NULL) {
                    continue;
                }
                MONSTER_Load_Monster(&monsterSet[idx], block, x, y, gene);
                idx++;
            }
        }
    }
}
static void MONSTER_Unload_MonsterSet() {
    if (monsterSet == NULL) {
        return;
    }
    for (int i = 0; i < lenMonsterSet; i++) {
        MONSTER_Unload_Monster(&monsterSet[i]);
    }
    free(monsterSet); // free
    monsterSet = NULL;
}


static void MONSTER_Print_MonsterSet() {
    printf("MONSTER SET[%d]:\n", lenMonsterSet);
    for (int i = 0; i < lenMonsterSet; i++) {
        const Monster monster = monsterSet[i];
        printf("%4d. %s\n", i, GENE_GetNameFromGene(monster.gene));
    }
}
void MONSTER_Load() {
    MONSTER_Load_MonsterSet();
    MONSTER_Print_MonsterSet();
}
void MONSTER_Unload() {
    MONSTER_Unload_MonsterSet();
}


void MONSTER_Renew() {

}
void MONSTER_Draw() {
}