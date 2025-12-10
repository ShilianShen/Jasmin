#include "bool.h"


static const float A = 4, B = 4, C = 6, D = 36;
static const SDL_Color color = {255, 255, 200, 255};
static void TEMPO_TrigFuncBool(const TrigPara para) {
    const TypeBool* bool_ = (TypeBool*)para;
    bool* now = bool_->now;
    if (now != NULL) *now = !*now;
}
bool createBool(void* info, const cJSON* info_json) {
    TypeBool* bool_ = info;
    REQ_CONDITION(cJSON_IsObject(info_json), return false);

    cJSON_LoadByKey(info_json, "readonly", JSM_BOOL, &bool_->readonly);

    const char* now_json = NULL;
    REQ_CONDITION(cJSON_LoadByKey(info_json, "now", JSM_STRING, &now_json), return false);
    bool_->now = BASIC_GetTableValByKey(TEMPO_PTR_TABLE, now_json);
    REQ_CONDITION(bool_->now != NULL, return false);

    const float W = 2 * A + 2 * B + D;
    const float H = 2 * A + 2 * B + D;
    bool_->texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, (int)W, (int)H);
    REQ_CONDITION(bool_->texture != NULL, return false);

    return true;
}
void deleteBool(void *info) {
    TypeBool* bool_ = info;
    SDL_DestroyTexture(bool_->texture);
}
SDL_Texture* textureBool(void* info) {
    const TypeBool* bool_ = info;
    const float M = 1;
    const float N = *bool_->now;
    const float W = 2 * A + (M + 1) * B + M * D;
    const float H = 2 * A + 2 * B + D;

    SDL_SetRenderTarget(renderer, bool_->texture);
    SDL_SetRenderColor(renderer, EMPTY);
    SDL_RenderClear(renderer);
    SDL_SetRenderColor(renderer, color);
    const SDL_FRect frame[] = {
        (SDL_FRect){0, 0, W, A},
        (SDL_FRect){0, 0, A, H},
        (SDL_FRect){0, H - A, W, A},
        (SDL_FRect){W - A, 0, A, H},
        (SDL_FRect){A + B, A + B, (W - 2 * A - 2 * B) * N / M, H - 2 * A - 2 * B},
    };
    SDL_RenderFillRects(renderer, frame, 5);
    SDL_SetRenderTarget(renderer, NULL);
    return bool_->texture;
}
bool trigBool(void* info, const SDL_FPoint mouse, const SDL_FRect dst_rect) {
    const TypeBool* bool_ = info;
    if (PERPH_GetMouseAndKeyInRect(PERPH_MOUSE_KEY_LEFT, dst_rect)) {
        PERPH_SetMouseKeyTrig(PERPH_MOUSE_KEY_LEFT, (Trig){TEMPO_TrigFuncBool, (TrigPara)bool_, false});
    }
    return true;
}
