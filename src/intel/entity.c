#include "entity.h"



Entity entitySet[NUM_ENTITIES] = {
    [ENTITY_UNKNOWN] = {.name = "unknown"},
    [ENTITY_SOCRATES] = {.name = "Socrates"},
    [ENTITY_HUMAN] = {.name = "Human"},
    [ENTITY_DEATH] = {.name = "Death"},
    [ENTITY_0] = {.name = "0000"},
    [ENTITY_1] = {.name = "1111"},
    [ENTITY_2] = {.name = "2222"},
    [ENTITY_3] = {.name = "3333"},
};


bool INTEL_InitEntity() {
    TTF_Font* font = TTF_OpenFont("../res/font/Courier New.ttf", 48);
    for (int i = 0; i < NUM_ENTITIES; i++) {
        const char* string = entitySet[i].name == NULL ? "????" : entitySet[i].name;
        entitySet[i].tex = TXT_LoadTexture(renderer, font, string, (SDL_Color){255, 255, 255, 255});
        REQ_CONDITION(entitySet[i].tex != NULL, {
            TTF_CloseFont(font); font = NULL;
            return false;
        });
    }
    TTF_CloseFont(font); font = NULL;
    return true;
}
void INTEL_ExitEntity() {
    for (int i = 0; i < NUM_ENTITIES; i++) {
        SDL_DestroyTexture(entitySet[i].tex);
        entitySet[i].tex = NULL;
    }
}