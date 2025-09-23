#include "slid.h"
#include "math.h"


static const float A = 4, B = 4, C = 6, D = 36;


bool TEMPO_CreateElemSlid(void* info, const cJSON* info_json) {
    ElemSlidInfo* slid = info;
    if (cJSON_IsObject(info_json) == false) {
        return false;
    }

    const char* key = NULL;
    const char* now_json = NULL;

    if (cJSON_Load(info_json, key = "discrete", JSM_BOOL, &slid->discrete) == false) {
        printf("%s: failed in %s.\n", __func__, key);
        return false;
    }
    if (cJSON_Load(info_json, key = "readonly", JSM_BOOL, &slid->readonly) == false) {
        printf("%s: failed in %s.\n", __func__, key);
        return false;
    }
    if (cJSON_Load(info_json, key = "min", JSM_FLOAT, &slid->min) == false) {
        printf("%s: failed in %s.\n", __func__, key);
        return false;
    }
    if (cJSON_Load(info_json, key = "max", JSM_FLOAT, &slid->max) == false) {
        printf("%s: failed in %s.\n", __func__, key);
        return false;
    }
    if (cJSON_Load(info_json, key = "now", JSM_STRING, &now_json) == false) {
        printf("%s: failed in %s.\n", __func__, key);
        return false;
    }

    if (slid->discrete) {
        slid->min = roundf(slid->min);
        slid->max = roundf(slid->max);
    }

    const JSM_DataType type = slid->discrete ? JSM_INT : JSM_FLOAT;
    slid->now = TABLE_GetValByKey(TEMPO_ExternTable[type], now_json);
    if (slid->now == NULL) {
        printf("%s: failed in %s.\n", __func__, key);
        return false;
    }

    return true;
}
bool TEMPO_RenewElemSlid(const void* info, SDL_Texture** tex) {
    const ElemSlidInfo* slid = info;
    const float M = slid->max - slid->min;
    const float W = 2 * A + (M + 1) * B + M * C;
    const float H = 2 * A + 2 * B + D;
    // elem->src_rect = (SDL_FRect){0, 0, W, H};
    // elem->gid_rect.w = elem->gid_rect.h = 1;
    *tex = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,
        (int)W, (int)H
        );
    if (*tex == NULL) {
        return false;
    }
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



void TrigFunc_Slid(const ElemSlidInfo* slid, const SDL_FRect dst_rect) {
    if (slid->readonly) {
        return;
    }
    const float min = dst_rect.x + A + B;
    const float max = dst_rect.x + dst_rect.w - A - B;
    const float now = DEVICE_GetMousePos().x;
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