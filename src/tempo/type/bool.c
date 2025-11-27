#include "bool.h"


static const float A = 4, B = 4, C = 6, D = 36;


bool TEMPO_CreateTypeBool(void* info, const cJSON* info_json) {
    TypeBoolInfo* bool_ = info;
    if (cJSON_IsObject(info_json) == false) {
        return false;
    }

    const char* now_json = NULL;
    REQ_CONDITION(cJSON_LoadByKey(info_json, "now", JSM_STRING, &now_json), return false);

    cJSON_LoadByKey(info_json, "readonly", JSM_BOOL, &bool_->readonly);

    bool_->now = BASIC_GetTableValByKey(TEMPO_PTR_TABLE, now_json);
    return true;
}
bool TEMPO_RenewTypeBool(const void* info, SDL_Texture** tex) {
    const TypeBoolInfo* bool_ = info;
    const float M = 1;
    const float N = *bool_->now;
    const float W = 2 * A + (M + 1) * B + M * D;
    const float H = 2 * A + 2 * B + D;

    if (*tex == NULL) {
        *tex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, (int)W, (int)H);
    }
    REQ_CONDITION(*tex != NULL, return false);

    SDL_SetRenderTarget(renderer, *tex);
    SDL_SetRenderDrawColor(renderer, 255, 255, 200, 255);
    const SDL_FRect frame[5] = {
        (SDL_FRect){0, 0, W, A},
        (SDL_FRect){0, 0, A, H},
        (SDL_FRect){0, H - A, W, A},
        (SDL_FRect){W - A, 0, A, H},
        (SDL_FRect){A + B, A + B, (W - 2 * A - 2 * B) * N / M, H - 2 * A - 2 * B},
    };
    SDL_RenderFillRects(renderer, frame, 5);
    SDL_SetRenderTarget(renderer, NULL);
    return true;
}