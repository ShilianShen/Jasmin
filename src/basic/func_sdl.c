#include "func.h"



SDL_FRect SDL_RoundFRect(const SDL_FRect rect) {
    const SDL_FRect result = {
        roundf(rect.x),
        roundf(rect.y),
        roundf(rect.w),
        roundf(rect.h),
    };
    return result;
}
char*SDL_GetStrColor(const SDL_Color color) {
    static char string[] = "[255, 255, 255, 255]";
    static size_t len = 0;
    if (len == 0) {
        len = sizeof(string);
    }
    snprintf(string, len, "[%3d, %3d, %3d, %3d]", color.r, color.g, color.b, color.a);
    return string;
}
char*SDL_GetStrFRect(const SDL_FRect rect) {
    static char string[32];
    snprintf(string, 31, "[%.2f, %.2f, %.2f, %.2f]", rect.x, rect.y, rect.w, rect.h);
    return string;
}
char*SDL_GetStrFPoint(const SDL_FPoint point) {
    static char string[32];
    snprintf(string, 31, "[%.2f, %.2f]", point.x, point.y);
    return string;
}
bool SDL_GetSurfaceColor(SDL_Surface* surface, const int x, const int y, SDL_Color* color) {
    REQ_CONDITION(surface != NULL, return false);
    REQ_CONDITION(color != NULL, return false);
    SDL_ReadSurfacePixel(surface, x, y, &color->r, &color->g, &color->b, &color->a);
    return true;
}
bool SDL_SetSurfaceColor(SDL_Surface* surface, const int x, const int y, const SDL_Color color) {
    REQ_CONDITION(surface != NULL, return false);
    SDL_WriteSurfacePixel(surface, x, y, color.r, color.g, color.b, color.a);
    return true;
}
SDL_FPoint SDL_GetSumFPoint(const int N, const SDL_FPoint points[N]) {
    SDL_FPoint sum = {0, 0};
    for (int i = 0; i < N; i++) {
        sum.x += points[i].x;
        sum.y += points[i].y;
    }
    return sum;
}

bool SDL_LoadDstRectAligned(
    SDL_FRect *dst,
    const SDL_Texture *texture,
    const SDL_FRect *src,
    const SDL_FRect *gid,
    const SDL_FRect *bck,
    const int anchor) {

    OPT_CONDITION(dst != NULL, return false);
    OPT_CONDITION(src != NULL || texture != NULL, return false);

    const SDL_FRect src_rect = src != NULL ? *src : (SDL_FRect){0, 0, (float)texture->w, (float)texture->h};
    const SDL_FRect gid_rect = gid != NULL ? *gid : (SDL_FRect){0, 0, 1, 1};
    const SDL_FRect bck_rect = bck != NULL ? *bck : windowRect;

    dst->w = src_rect.w * gid_rect.w;
    dst->h = src_rect.h * gid_rect.h;

    const int x1 = (anchor + 40) % 9 / 3;
    const int y1 = (anchor + 40) / 9 / 3;
    const int x2 = (anchor + 40) % 9 % 3;
    const int y2 = (anchor + 40) / 9 % 3;

    const SDL_FPoint center = {
        bck_rect.x + bck_rect.w / 2 * (float)x1,
        bck_rect.y + bck_rect.h / 2 * (float)y1,
    };
    const SDL_FPoint adjust = {
        -dst->w * (1 - (float)x2 / 2),
        -dst->h * (1 - (float)y2 / 2),
    };

    dst->x = center.x + adjust.x + gid_rect.x;
    dst->y = center.y + adjust.y + gid_rect.y;

    return true;
}
bool SDL_RenderTextureAligned(
    SDL_Renderer* renderer,
    SDL_Texture* texture,
    const SDL_FRect* src_rect,
    const SDL_FRect* gid_rect,
    const SDL_FRect* bck_rect,
    const int anchor
    ) {
    REQ_CONDITION(renderer != NULL, return false);
    REQ_CONDITION(texture != NULL, return false);

    SDL_FRect dst_rect;
    SDL_LoadDstRectAligned(&dst_rect, texture, src_rect, gid_rect, bck_rect, anchor);
    return SDL_RenderTexture(renderer, texture, src_rect, &dst_rect);
}
bool SDL_RenderText(SDL_Renderer* renderer, TTF_Font* font, const char* text, const SDL_FPoint point, const SDL_Color color) {
    OPT_CONDITION(renderer != NULL && text != NULL && font != NULL, return false);
    SDL_Texture* texture = TXT_LoadTexture(renderer, font, text, color);
    OPT_CONDITION(texture != NULL, return false);
    SDL_RenderTexture(renderer, texture, NULL, &(SDL_FRect){point.x, point.y, (float)texture->w, (float)texture->h});
    SDL_DestroyTexture(texture);
    return true;
}


bool SDL_GetPointInRect(const SDL_FPoint point, const SDL_FRect rect) {
    return rect.x <= point.x && point.x < rect.x + rect.w && rect.y <= point.y && point.y < rect.y + rect.h;
}
SDL_Texture* TXT_LoadTexture(SDL_Renderer* renderer, TTF_Font* font, const char* text, const SDL_Color color) {
    REQ_CONDITION(renderer != NULL && font != NULL && text != NULL, return NULL);
    if (strlen(text) == 0) {return NULL;} // Opt

    // getSurface N-Condition
    SDL_Surface* surface = TTF_RenderText_Blended(font, text, 0, color);
    if (surface == NULL) {printf("%s: fail to create surface from \"%s\".\n", __func__, text); return NULL;}

    // getTexture N-Condition
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);
    if (texture == NULL) {printf("%s: fail to create texture from \"%s\".\n", __func__, text); return NULL;}

    return texture;
}
SDL_Texture* TXT_LoadTextureWithLines(
    SDL_Renderer* renderer,
    TTF_Font* font,
    const char* scr_text,
    const SDL_Color colorText,
    const SDL_Color colorBack,
    const char aligned
    ) {

    if (strlen(scr_text) == 0) return NULL; // Opt Condition
    int size_text = (int)strlen(scr_text) + 1;
    char text[size_text];
    strcpy(text, scr_text);

    int num_lines = 1;
    for (int i = 0; text[i] != '\0'; i++) {
        if (text[i] == '\n') {
            num_lines++;
        }
    }

    int line_offsets[num_lines];
    line_offsets[0] = 0;
    for (int i = 0, j = 1; text[i] != '\0'; i++) {
        // i: index of char, j: index of line
        if (text[i] == '\n') {
            line_offsets[j] = i + 1;
            text[i] = '\0';
            j++;
        }
    }

    SDL_Texture* sub_textures[num_lines];
    for (int i = 0; i < num_lines; i++) {
        const char* sub_text = text + line_offsets[i];
        if (strlen(sub_text) == 0) continue;
        sub_textures[i] = TXT_LoadTexture(renderer, font, sub_text, colorText);  // alloc
        if (sub_textures[i] == NULL) {
            printf("%s: fail to create texture from \"%s\".\n", __func__, sub_text);
        }
    }

    // getMainTextureSize
    const float line_height = (float)TTF_GetFontHeight(font);
    const int main_texture_height = num_lines * TTF_GetFontHeight(font);
    float line_widths[num_lines];
    int main_texture_width = 0;
    for (int i = 0; i < num_lines; i++) {
        SDL_GetTextureSize(sub_textures[i], &line_widths[i], NULL);
        if ((float)main_texture_width < line_widths[i]) {main_texture_width = (int)line_widths[i];}
    }

    // getTexture N-Condition
    SDL_Texture* texture = SDL_CreateTexture(
        renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
        main_texture_width, main_texture_height
        );
    if (texture == NULL) {printf("%s: fail to create texture from \"%s\".\n", __func__, text); return NULL;}
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND_PREMULTIPLIED);

    // clearTexture
    SDL_SetRenderTarget(renderer, texture);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    // updateTexture
    SDL_SetRenderColor(renderer, colorBack);
    for (int i = 0; i < num_lines; i++) {
        SDL_FRect dst_rect = {0, line_height * (float)i, line_widths[i], line_height};
        switch (aligned) {
            case 'r': case 'R': {dst_rect.x = (float)main_texture_width - dst_rect.w; break;}
            case 'c': case 'C': {dst_rect.x = ((float)main_texture_width - dst_rect.w) / 2; break;}
            case 'l': case 'L': default: {break;}
        }
        SDL_RenderFillRect(renderer, &dst_rect);
        SDL_RenderTexture(renderer, sub_textures[i], NULL, &dst_rect);
    }

    // end
    for (int i = 0; i < num_lines; i++) {SDL_DestroyTexture(sub_textures[i]);}
    SDL_SetRenderTarget(renderer, NULL);

    return texture;
}
SDL_FColor SDL_GetFColorFromColor(const SDL_Color color) {
    return (SDL_FColor){(float)color.r / 255, (float)color.g / 255, (float)color.b / 255, (float)color.a / 255};
}
bool SDL_SetRenderColor(SDL_Renderer* renderer, const SDL_Color color) {
    return SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}
bool SDL_GetColorEqual(const SDL_Color x, const SDL_Color y) {
    return x.r == y.r && x.g == y.g && x.b == y.b && x.a == y.a;
}
bool SDL_RenderParallelLine(SDL_Renderer *renderer, const SDL_FPoint A, const SDL_FPoint B, const float offset) {
    // 计算线段方向
    const float dx = B.x - A.x, dy = B.y - A.y;
    const float len = sqrtf(dx * dx + dy * dy);
    if (len < 1e-6f) return false; // 避免除零

    // 单位法向量（垂直方向）
    const float nx = -dy / len, ny = dx / len;

    // 平行线偏移向量
    const float ox = nx * (offset / 2.0f), oy = ny * (offset / 2.0f);

    return true
    && SDL_RenderLine(renderer, A.x + ox, A.y + oy, B.x + ox, B.y + oy)
    && SDL_RenderLine(renderer, A.x - ox, A.y - oy, B.x - ox, B.y - oy)
    ;
}
bool SDL_RenderDashedLine(SDL_Renderer *renderer, SDL_FPoint A, SDL_FPoint B, float dash_len, float gap_len, float offset) {
    // 确保长度参数有效
    if (dash_len <= 0.0f) dash_len = 1.0f;
    if (gap_len < 0.0f) gap_len = 0.0f;

    // 计算线段方向和长度
    float dx = B.x - A.x;
    float dy = B.y - A.y;
    float total_len = sqrtf(dx * dx + dy * dy);
    if (total_len < 1e-6f) return false;

    // 单位方向向量
    float ux = dx / total_len;
    float uy = dy / total_len;

    // 从 offset 开始
    float pos = fmodf(offset, dash_len + gap_len);
    if (pos < 0.0f) pos += dash_len + gap_len;

    float x = A.x + ux * pos;
    float y = A.y + uy * pos;
    float remaining = total_len - pos;

    while (remaining > 0.0f) {
        float segment = fminf(dash_len, remaining);
        float x2 = x + ux * segment;
        float y2 = y + uy * segment;

        // 绘制实线段
        SDL_RenderLine(renderer, x, y, x2, y2);

        // 跳过空白段
        x = x2 + ux * gap_len;
        y = y2 + uy * gap_len;

        remaining -= (dash_len + gap_len);
    }
    return true;
}
bool SDL_SetTextureColorRGB(SDL_Texture* tex, const SDL_Color color) {
    return SDL_SetTextureColorMod(tex, color.r, color.g, color.b);
}
SDL_FPoint SDL_ClipPointInRect(const SDL_FPoint point, const SDL_FRect rect) {
    if (SDL_GetPointInRect(point, rect)) return point;
    SDL_FPoint result = point;
    if (point.x < rect.x) result.x = rect.x;
    if (point.y < rect.y) result.y = rect.y;
    if (rect.x + rect.w < point.x) result.x = rect.x + rect.w;
    if (rect.y + rect.h < point.y) result.y = rect.y + rect.h;
    return result;
}
bool SDL_GetPointInTexture(const SDL_FPoint point, const SDL_Texture* texture) {
    if (texture == NULL) return false;
    const SDL_FRect rect = {0, 0, (float)texture->w, (float)texture->h};
    return SDL_GetPointInRect(point, rect);
}
SDL_FPoint SDL_StdPointByRect(SDL_FPoint point, const SDL_FRect rect) {
    point.x = (point.x - rect.x) / rect.w;
    point.y = (point.y - rect.y) / rect.h;
    return point;
}
SDL_FPoint SDL_ScaleByTexture(SDL_FPoint point, const SDL_Texture* texture) {
    if (texture == NULL) return (SDL_FPoint){0, 0};
    point.x *= (float)texture->w;
    point.y *= (float)texture->h;
    return point;
}