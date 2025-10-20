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
static const char* FONT_PATH = "../res/font/IBMPlexMono-Medium.ttf";
static const float FONT_SIZE = 24;


bool INTEL_InitAction() {
    TTF_Font* font = TTF_OpenFont(FONT_PATH, FONT_SIZE);
    REQ_CONDITION(font != NULL, return false);
    for (int i = 0; i < NUM_ACTIONS; i++) {
        const char* string = actionSet[i].name == NULL ? "????" : actionSet[i].name;
        actionSet[i].tex = TXT_LoadTexture(renderer, font, string, (SDL_Color){255, 255, 255, 255});
        REQ_CONDITION(actionSet[i].tex != NULL, {
            TTF_CloseFont(font); font = NULL;
            return false;
        });
    }
    TTF_CloseFont(font); font = NULL;
    return true;
}
void INTEL_ExitAction() {
    for (int i = 0; i < NUM_ACTIONS; i++) {
        SDL_DestroyTexture(actionSet[i].tex); actionSet[i].tex = NULL;
    }
}