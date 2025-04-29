#ifndef BASIC_H
#define BASIC_H


// display parameters
typedef float Pixel;
int logical_w, logical_h;
int windowWidth, windowHeight;
float scale_x = 1, scale_y = 1;

void renewScreenParas(SDL_Window* window) {
    SDL_GetWindowSize(window, &logical_w, &logical_h);
    SDL_GetWindowSizeInPixels(window, &windowWidth, &windowHeight);
    scale_x = (float)windowWidth / (float)logical_w;
    scale_y = (float)windowHeight / (float)logical_h;
}


// 基本函数
SDL_Texture* TXT_LoadTexture(SDL_Renderer* renderer, TTF_Font* font, const char* text, const SDL_Color color) {
    // 检查参数
    if (renderer == NULL) {
        printf("TXT_LoadTexture: Renderer[%p] not exists.\n", renderer);
        return NULL;
    }
    if (font == NULL) {
        printf("TXT_LoadTexture: Font[%p] not exists.\n", font);
        return NULL;
    }
    if (text == NULL) {
        printf("TXT_LoadTexture: Text[%p] not exists.\n", text);
        return NULL;
    }


    // 创建 surface
    SDL_Surface* surface = TTF_RenderText_Blended(font, text, 0, color);
    if (surface == NULL) {
        printf("TXT_LoadTexture: Fail to create surface from %s.\n", text);
        return NULL;
    }

    // 创建 texture
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);
    if (texture == NULL) {
        printf("TXT_LoadTexture: Fail to create texture from %s.\n", text);
        return NULL;
    }

    return texture;
}
void SDL_SetRenderSDLColor(SDL_Renderer* renderer, const SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}


// EASE
float EASE_bySin(const float x) {
    if (x >= 1) {return 1;}
    if (x <= 0) {return 0;}
    return SDL_pow(SDL_sinf(3.14 / 2 * x), 2);
}


#endif //BASIC_H
