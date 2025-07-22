#include "setting.h"


int logical_w, logical_h;
int windowWidth, windowHeight;
float scale_x = 1, scale_y = 1;
SDL_Color EMPTY = {0, 0, 0, 0};


toml_table_t* getToml(const char* tomlPath) {
    // Req Condition
    FILE* file = fopen(tomlPath, "r"); // malloc
    if (file == NULL) {printf("%s: failed to open \"%s\".\n", __func__, tomlPath); return NULL;}

    //
    toml_table_t* toml = toml_parse_file(file, NULL, 0); // malloc
    fclose(file); // end malloc

    // Req Condition
    if (toml == NULL) {printf("%s: failed to read \"%s\".\n", __func__, tomlPath); return NULL;}

    return toml;
}
void renewScreenParas(SDL_Window* window) {
    SDL_GetWindowSize(window, &logical_w, &logical_h);
    SDL_GetWindowSizeInPixels(window, &windowWidth, &windowHeight);
    scale_x = (float)windowWidth / (float)logical_w;
    scale_y = (float)windowHeight / (float)logical_h;
}
SDL_Texture* TXT_LoadTexture(SDL_Renderer* renderer, TTF_Font* font, const char* text, const SDL_Color color) {
    // N-Condition
    if (renderer == NULL) {printf("%s: renderer[%p] not exists.\n", __func__, renderer); return NULL;}
    if (font == NULL) {printf("%s: font[%p] not exists.\n", __func__, font); return NULL;}
    if (text == NULL) {printf("%s: text[%p] not exists.\n", __func__, text); return NULL;}

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
    // get text
    char* text = strdup(scr_text);

    // get num_lines (Opt Condition)
    if (strlen(scr_text) == 0) { return NULL; }
    int num_lines = 1;
    for (int i = 0; text[i+1] != '\0'; i++) {
        if (text[i] == '\n') { num_lines++; }
    }

    // get line_offsets
    int line_offsets[num_lines];
    line_offsets[0] = 0;
    for (int i = 0, j = 1; text[i] != '\0'; i++) {
        // i: index of char, j: index of line
        if (text[i] == '\n') {
            line_offsets[j] = i+1;
            text[i] = '\0';
            j++;
        }
    }

    // get sub_textures (Req Condition)
    SDL_Texture* sub_textures[num_lines];
    for (int i = 0; i < num_lines; i++) {
        sub_textures[i] = TXT_LoadTexture(renderer, font, text + line_offsets[i], colorText);  // malloc
        if (sub_textures[i] == NULL) {
            printf("%s: fail to create texture from \"%s\".\n", __func__, text + line_offsets[i]);
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
    SDL_SetRenderSDLColor(renderer, colorBack);
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
    free(text);
    for (int i = 0; i < num_lines; i++) {SDL_DestroyTexture(sub_textures[i]);}
    SDL_SetRenderTarget(renderer, NULL);

    return texture;
}
bool SDL_SetRenderSDLColor(SDL_Renderer* renderer, const SDL_Color color) {
    return SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}
bool SDL_SetRenderSDLColorAlpha(SDL_Renderer* renderer, const SDL_Color color, const Uint8 alpha) {
    return SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, alpha);
}


void SDL_PrintFRect(const SDL_FRect rect) {
    printf("[%.2f, %.2f, %.2f, %.2f]", rect.x, rect.y, rect.w, rect.h);
}
void SDL_LoadDstRectAligned(
    SDL_FRect* dst_rect,
    SDL_Texture* texture,
    const SDL_FRect* src_rect,
    const SDL_FRect* gid_rect,
    const SDL_FRect* bck_rect,
    const int anchor
    ) {
    // Req Condition
    if (dst_rect == NULL) {printf("%s: dst_rect not exists.\n", __func__); return;}
    if (texture == NULL) {printf("%s: fail to get texture.\n", __func__); return;}

    // load src (Opt Condition)
    SDL_FRect src = {0, 0, 0, 0};
    if (src_rect != NULL) {
        src.w = src_rect->w;
        src.h = src_rect->h;
    }
    else {SDL_GetTextureSize(texture, &src.w, &src.h);}

    // load gid (Opt Condition)
    SDL_FRect gid = {0, 0, 1, 1};
    if (gid_rect != NULL) {gid = *gid_rect;}

    // load bck (Opt Condition)
    SDL_FRect bck = {0, 0, (float)windowWidth, (float)windowHeight};
    if (bck_rect != NULL) {bck = *bck_rect;}

    // load dst_rect -> w, h
    dst_rect->w = src.w * gid.w;
    dst_rect->h = src.h * gid.h;

    // load dst_rect -> x, y
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