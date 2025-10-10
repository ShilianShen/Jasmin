#include "action.h"


Action actionSet[NUM_ACTIONS] = {
    [ACTION_UNKNOWN] = {.name = "unknown"},
    [ACTION_EQUAL] = {.name = "equal"},
    [ACTION_BELONG] = {.name = "belong"},
    [ACTION_WILL] = {.name = "will"},
    [ACTION_0] = {.name = "ACTION_0"},
    [ACTION_1] = {.name = "ACTION_1"},
    [ACTION_2] = {.name = "ACTION_2"},
    [ACTION_3] = {.name = "ACTION_3"},
};


bool INTEL_InitAction() {
    TTF_Font* font = TTF_OpenFont("../res/font/JetBrainsMono-Regular.ttf", 24);
    for (int i = 0; i < NUM_ACTIONS; i++) {
        const char* string = actionSet[i].name == NULL ? "????" : actionSet[i].name;
        actionSet[i].tex = TXT_LoadTexture(renderer, font, string, (SDL_Color){0, 0, 0, 255});
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