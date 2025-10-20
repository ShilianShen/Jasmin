#include "action.h"
#include "entity.h"
#include "intel_arr.h"


Action actionSet[NUM_ACTIONS] = {
    [ACTION_NULL] = {.name = "unknown", .type = ACTION_TYPE_ONE_WAY},
    [ACTION_IS] = {.name = "is", .type = ACTION_TYPE_TWO_WAY},
    [ACTION_BELONG] = {.name = "belong", .type = ACTION_TYPE_ONE_WAY},
    [ACTION_WILL] = {.name = "will", .type = ACTION_TYPE_ONE_WAY},
    [ACTION_CAN] = {.name = "can", .type = ACTION_TYPE_ONE_WAY},
};


bool INTEL_InitAction() {
    for (int i = 0; i < NUM_ACTIONS; i++) {
        const char* string = actionSet[i].name == NULL ? "????" : actionSet[i].name;

        actionSet[i].netTex = TXT_LoadTexture(renderer, actionFont, string, (SDL_Color){255, 255, 255, 255});
        REQ_CONDITION(actionSet[i].netTex != NULL, return false);

        actionSet[i].setTex = TXT_LoadTexture(renderer, setFont, string, (SDL_Color){255, 255, 255, 255});
        REQ_CONDITION(actionSet[i].setTex != NULL, return false);
    }
    return true;
}
void INTEL_ExitAction() {
    for (int i = 0; i < NUM_ACTIONS; i++) {
        SDL_DestroyTexture(actionSet[i].netTex); actionSet[i].netTex = NULL;
    }
}