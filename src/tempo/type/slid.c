#include "slid.h"
#include "math.h"


static const float A = 4, B = 4, C = 6, D = 36;


bool TEMPO_CreateTypeSlid(void* info, const cJSON* info_json) {
    TypeSlidInfo* slid = info;
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

    const JSM_DataType type = slid->discrete ? JSM_INT : JSM_FLOAT;
    slid->now = BASIC_GetTableValByKey(TEMPO_PTR_TABLE, now_json);
    REQ_CONDITION(slid->now != NULL, return false);

    return true;
}
bool TEMPO_RenewTypeSlid(const void* info, SDL_Texture** tex) {
    const TypeSlidInfo* slid = info;
    const float M = slid->max - slid->min;
    const float W = 2 * A + (M + 1) * B + M * C;
    const float H = 2 * A + 2 * B + D;
    // elem->src_rect = (SDL_FRect){0, 0, W, H};
    // elem->gid_rect.w = elem->gid_rect.h = 1;
    if (*tex == NULL) {
        *tex = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,
        (int)W, (int)H
        );
    }
    REQ_CONDITION(*tex != NULL, return false);
    SDL_SetRenderTarget(renderer, *tex);
    SDL_SetRenderDrawColor(renderer, 255, 255, 200, 255);
    const SDL_FRect frame[4] = {
        (SDL_FRect){0, 0, W, A},
        (SDL_FRect){0, 0, A, H},
        (SDL_FRect){0, H - A, W, A},
        (SDL_FRect){W - A, 0, A, H},
    };
    SDL_RenderFillRects(renderer, frame, 4);
    if (slid->discrete) {
        if (slid->now != NULL) {
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
            SDL_RenderLine(renderer, 0, 0, W, H);
            SDL_RenderLine(renderer, W, 0, 0, H);
        }
    }
    else {
        if (slid->now != NULL) {
            const float N = *slid->now - slid->min;
            const SDL_FRect rect = {A + B, A + B, (W - 2 * A - 2 * B) * N / M, H - 2 * A - 2 * B};
            SDL_RenderFillRect(renderer, &rect);
        }
        else {
            SDL_RenderLine(renderer, 0, 0, W, H);
            SDL_RenderLine(renderer, W, 0, 0, H);
        }
    }
    SDL_SetRenderTarget(renderer, NULL);
    return true;
}



void TrigFunc_Slid(const TypeSlidInfo* slid, const SDL_FRect dst_rect) {
    if (slid->readonly) {
        return;
    }
    const float min = dst_rect.x + A + B;
    const float max = dst_rect.x + dst_rect.w - A - B;
    const float now = PERPH_GetMousePos().x;
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