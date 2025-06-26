#include "setting.h"


int logical_w, logical_h;
int windowWidth, windowHeight;
float scale_x = 1, scale_y = 1;


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
SDL_Texture* TXT_LoadTextureWithLines(SDL_Renderer* renderer, TTF_Font* font, const char* scr_text, const SDL_Color color, const char aligned) {
    // getText
    char text[strlen(scr_text)];
    strcpy(text, scr_text);

    // getNumLines N-Condition
    int num_lines = 1;
    for (int i = 0; text[i] != '\0'; i++) {
        if (text[i] == '\n') {
            num_lines++;
        }
    }
    if (num_lines == 1) {return TXT_LoadTexture(renderer, font, scr_text, color);}

    // getLineHeads
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

    // getSubTextures N-Condition
    SDL_Texture* sub_textures[num_lines];
    for (int i = 0; i < num_lines; i++) {
        sub_textures[i] = TXT_LoadTexture(renderer, font, text + line_offsets[i], color);  // malloc
        if (sub_textures[i] == NULL) {
            // U-Condition
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
    for (int i = 0; i < num_lines; i++) {
        SDL_FRect dst_rect = {0, line_height * (float)i, line_widths[i], line_height};
        switch (aligned) {
            case 'r': case 'R': {dst_rect.x = (float)main_texture_width - dst_rect.w; break;}
            case 'c': case 'C': {dst_rect.x = ((float)main_texture_width - dst_rect.w) / 2; break;}
            case 'l': case 'L': default: {break;}
        }
        SDL_RenderTexture(renderer, sub_textures[i], NULL, &dst_rect);
    }

    // end
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