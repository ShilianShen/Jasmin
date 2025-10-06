#include "func.h"



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
    SDL_ReadSurfacePixel(surface, x, y, &color->r, &color->g, &color->b, &color->a);
    return true;
}

bool SDL_LoadDstRectAligned(SDL_FRect *dst_rect, SDL_Texture *texture, const SDL_FRect *src_rect, const SDL_FRect *gid_rect, const SDL_FRect *bck_rect, int anchor) {
    REQ_CONDITION(dst_rect != NULL, return false);


    SDL_FRect src = {0, 0, 0, 0};
    if (texture == NULL && src_rect == NULL) {
        src.w = (float)windowWidth;
        src.h = (float)windowHeight;
    }
    else if (src_rect != NULL) {
        src.w = src_rect->w;
        src.h = src_rect->h;
    }
    else {SDL_GetTextureSize(texture, &src.w, &src.h);}


    SDL_FRect gid = {0, 0, 1, 1};
    if (gid_rect != NULL) {gid = *gid_rect;}


    SDL_FRect bck = {0, 0, (float)windowWidth, (float)windowHeight};
    if (bck_rect != NULL) {bck = *bck_rect;}


    dst_rect->w = src.w * gid.w;
    dst_rect->h = src.h * gid.h;

    // load dst_rect -> x, y
    anchor += 40;
    const int x = anchor % 9;
    const int y = anchor / 9;
    float cx = 0, cy = 0, dx = 0, dy = 0;
    switch (x / 3) {
        case 0: cx = bck.x            ; break;
        case 1: cx = bck.x + bck.w / 2; break;
        case 2: cx = bck.x + bck.w    ; break;
        default: break;
    }
    switch (y / 3) {
        case 0: cy = bck.y            ; break;
        case 1: cy = bck.y + bck.h / 2; break;
        case 2: cy = bck.y + bck.h    ; break;
        default: break;
    }
    switch (x % 3) {
        case 0: dx = -dst_rect->w    ; break;
        case 1: dx = -dst_rect->w / 2; break;
        case 2: dx = 0               ; break;
        default: break;
    }
    switch (y % 3) {
        case 0: dy = -dst_rect->h    ; break;
        case 1: dy = -dst_rect->h / 2; break;
        case 2: dy = 0               ; break;
        default: break;
    }
    dst_rect->x = cx + dx + gid.x;
    dst_rect->y = cy + dy + gid.y;
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
    // Req Condition
    if (renderer == NULL) {printf("%s: renderer not exists.\n", __func__); return false;}
    if (texture == NULL) {printf("%s: texture  not exists.\n", __func__); return false;}

    //
    SDL_FRect dst_rect;
    SDL_LoadDstRectAligned(&dst_rect, texture, src_rect, gid_rect, bck_rect, anchor);
    return SDL_RenderTexture(renderer, texture, src_rect, &dst_rect);
}


bool SDL_GetPointInRect(const SDL_FPoint point, const SDL_FRect rect) {
    return rect.x <= point.x && point.x < rect.x + rect.w && rect.y <= point.y && point.y < rect.y + rect.h;
}
SDL_Texture* TXT_LoadTexture(SDL_Renderer* renderer, TTF_Font* font, const char* text, const SDL_Color color) {
    // N-Condition
    if (renderer == NULL) {printf("%s: renderer[%p] not exists.\n", __func__, renderer); return NULL;}
    if (font == NULL) {printf("%s: font[%p] not exists.\n", __func__, font); return NULL;}
    if (text == NULL) {printf("%s: text[%p] not exists.\n", __func__, text); return NULL;}
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
