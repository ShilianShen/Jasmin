#include "monster.h"


// MONSTER =============================================================================================================
static void MONSTER_Load_Monster(Monster* monster, Room* block, const int x, const int y, Gene* gene) {
    if (monster == NULL) {
        printf("%s, f.\n", __func__);
        return;
    } // Req Condition
    monster->gene = gene;
}
static void MONSTER_Unload_Monster(Monster* monster) {
}


// MONSTER SET =========================================================================================================
Monster* monsterSet = NULL;
Monster* you = NULL;
const char youName[] = "You";
int lenMonsterSet = 0;


static void MONSTER_LoadLenMonsterSet() {
    bool findYou = false;
    for (int i = 0; i < lenRoomSet; i++) {
        const Room* room = &roomSet[i];

        for (int x = 2; x < room->w-2; x++) {
            for (int y = 2; y < room->h-2; y++) {
                SDL_Color color;
                SDL_ReadSurfaceSDLColor(room->surface, x, y, &color);
                const Gene* gene = GENE_FindGeneFromColor(color);
                if (gene == NULL) continue;
                if (strcmp(GENE_GetNameFromGene(gene), youName) == 0) {
                    if (findYou == false) findYou = true;
                    else continue;
                }
                lenMonsterSet++;
            }
        }
    }
    if (lenMonsterSet == 0) {
        printf("%s, f.\n", __func__);
    }
}
static void MONSTER_Load_MonsterSet() {
    //
    MONSTER_LoadLenMonsterSet();
    monsterSet = malloc(lenMonsterSet * sizeof(Monster)); // malloc
    if (monsterSet == NULL) {
        printf("%s, f.\n", __func__);
        return;
    } // Req Condition
    //
    bool findYou = false;
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
                if (gene == NULL) continue;
                if (strcmp(GENE_GetNameFromGene(gene), youName) == 0) {
                    if (findYou == false) {
                        findYou = true;
                        you = &monsterSet[idx];
                    }
                    else continue;
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


// ?
static void MONSTER_Print_MonsterSet() {
    printf("MONSTER SET[%d]:\n", lenMonsterSet);
    for (int i = 0; i < lenMonsterSet; i++) {
        const Monster monster = monsterSet[i];
        printf("%4d. %s\n", i, GENE_GetNameFromGene(monster.gene));
    }
}


// LOAD & UNLOAD =======================================================================================================
void MONSTER_Load() {
    MONSTER_Load_MonsterSet();
    MONSTER_Print_MonsterSet();
}
void MONSTER_Unload() {
    MONSTER_Unload_MonsterSet();
}


// OTHER ===============================================================================================================
void MONSTER_Move(Monster* monster, const Direction direction) {
    if (monster == NULL) {
        printf("%s, f.\n", __func__);
        return;
    }
    switch (direction) {
        case DIRECTION_W: {
            monster->now.y -= 1;
            break;
        }
        case DIRECTION_A: {
            monster->now.x -= 1;
            break;
        }
        case DIRECTION_S: {
            monster->now.y += 1;
            break;
        }
        case DIRECTION_D: {
            monster->now.x += 1;
            break;
        }
        default: break;
    }
}
void MONSTER_Heal(Monster* monster) {
    if (monster == NULL) {
        printf("%s, f.\n", __func__);
        return;
    }
}

// RENEW ===============================================================================================================
void MONSTER_Renew() {

}