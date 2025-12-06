#include "slid.h"


static const float A = 4, B = 4, C = 6, D = 36;
static const SDL_Color color = {255, 255, 200, 255};
static void TrigFunc_Slid(const TrigPara para) {
    const TypeSlid* slid = (TypeSlid*)para;
    if (slid->readonly) return;

    const float min = A + B;
    const float max = (float)slid->texture->w - A - B;
    const float now = slid->point.x;
    if (slid->discrete) {
        if (now <= min)
            *(int*)slid->now = (int)slid->min;
        else if (now >= max)
            *(int*)slid->now = (int)slid->max;
        else
            *(int*)slid->now = (int)((now - min) / (B + C) + slid->min);
    }
    else {
        if (now <= min)
            *slid->now = slid->min;
        else if (now >= max)
            *slid->now = slid->max;
        else
            *slid->now = slid->min + (slid->max - slid->min) * (now - min) / (max - min);
    }
}
bool TEMPO_CreateTypeSlid(void* info, const cJSON* info_json) {
    TypeSlid* slid = info;
    REQ_CONDITION(cJSON_IsObject(info_json), return false);
    REQ_CONDITION(cJSON_LoadByKey(info_json, "discrete", JSM_BOOL, &slid->discrete), return false);
    REQ_CONDITION(cJSON_LoadByKey(info_json, "readonly", JSM_BOOL, &slid->readonly), return false);

    const char* now_json = NULL;
    REQ_CONDITION(cJSON_LoadByKey(info_json, "min", JSM_FLOAT, &slid->min), return false);
    REQ_CONDITION(cJSON_LoadByKey(info_json, "max", JSM_FLOAT, &slid->max), return false);
    REQ_CONDITION(cJSON_LoadByKey(info_json, "now", JSM_STRING, &now_json), return false);

    if (slid->discrete) {
        slid->min = roundf(slid->min);
        slid->max = roundf(slid->max);
    }
    slid->now = BASIC_GetTableValByKey(TEMPO_PTR_TABLE, now_json);
    REQ_CONDITION(slid->now != NULL, return false);

    const float M = slid->max - slid->min;
    const float W = 2 * A + (M + 1) * B + M * C;
    const float H = 2 * A + 2 * B + D;
    slid->texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, (int)W, (int)H);
    REQ_CONDITION(slid->texture != NULL, return false);

    return true;
}
bool TEMPO_RenewTypeSlid(void* info, SDL_Texture** tex, SDL_FPoint mouse) {
    TypeSlid* slid = info;
    const float M = slid->max - slid->min;
    const float W = (float)slid->texture->w;
    const float H = (float)slid->texture->h;
    *tex = slid->texture;
    SDL_SetRenderTarget(renderer, slid->texture);
    SDL_SetRenderColor(renderer, EMPTY);
    SDL_RenderClear(renderer);
    SDL_SetRenderColor(renderer, color);
    const SDL_FRect frame[] = {
        (SDL_FRect){0, 0, W, A},
        (SDL_FRect){0, 0, A, H},
        (SDL_FRect){0, H - A, W, A},
        (SDL_FRect){W - A, 0, A, H},
    };
    SDL_RenderFillRects(renderer, frame, len_of(frame));
    REQ_CONDITION(slid->now != NULL, return false);
    if (slid->discrete) {
        const int N = *(int*)slid->now - (int)slid->min;
        SDL_FRect rects[N];
        for (int i = 0; i < N; i++) {
            rects[i].x = A + (float)(i + 1) * B + (float)i * C;
            rects[i].y = A + B;
            rects[i].w = C;
            rects[i].h = D;
        }
        SDL_RenderFillRects(renderer, rects, N);
    }
    else {
        const float N = *slid->now - slid->min;
        const SDL_FRect rect = {A + B, A + B, (W - 2 * A - 2 * B) * N / M, H - 2 * A - 2 * B};
        SDL_RenderFillRect(renderer, &rect);
    }
    SDL_SetRenderTarget(renderer, NULL);
    if (&mouse != NULL && SDL_GetPointInTexture(mouse, *tex)) {
        slid->point = mouse;
        PERPH_SetMouseKeyTrig(PERPH_MOUSE_KEY_LEFT, (Trig){TrigFunc_Slid, (TrigPara)slid, true});
    }
    return true;
}
void TEMPO_DeleteTypeSlid(void* info) {
    const TypeSlid* slid = (TypeSlid*)info;
    SDL_DestroyTexture(slid->texture);
}

