#include "lotri.h"


SDL_Texture* LOTRI_texture = NULL;


bool LOTRI_Init() {
    cJSON* lotri_json = getJson(LOTRI_JSON);
    REQ_CONDITION(lotri_json != NULL, return false);

    int width = 0, height = 0;
    REQ_CONDITION(cJSON_LoadByKey(lotri_json, "width", JSM_INT, &width), return false);
    REQ_CONDITION(cJSON_LoadByKey(lotri_json, "height", JSM_INT, &height), return false);

    cJSON_free(lotri_json);

    LOTRI_texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,
        width,
        height
        );
    REQ_CONDITION(LOTRI_texture != NULL, return false);

    SDL_SetTextureScaleMode(LOTRI_texture, SDL_SCALEMODE_NEAREST);
    return true;
}
bool LOTRI_Renew() {
    LOTRI_ClearBuffer();
    return true;
}
bool LOTRI_Draw() {
    LOTRI_RenewCamera();
    LOTRI_DrawCamera();
    LOTRI_DrawBuffer();
    return true;
}
void LOTRI_Exit() {
    SDL_DestroyTexture(LOTRI_texture);
}