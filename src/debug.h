#ifndef DEBUG_H
#define DEBUG_H


// debug parameters
struct {
    TTF_Font* fontSmall;
    TTF_Font* fontBig;
    bool on;
    SDL_Color colorPoint;
    SDL_Color colorLine;
    SDL_Color colorFace;
    SDL_Color colorText;
    SDL_Color colorDark;
    SDL_Color colorLight;
} debug;
void loadDebugTheme() {
    debug.fontSmall = TTF_OpenFont("../fonts/JetBrainsMono-ExtraBold.ttf", 64);
    debug.fontBig = TTF_OpenFont("../fonts/JetBrainsMono-ExtraBold.ttf", 128);
    if (debug.fontSmall == NULL) {printf("Fail to load font.\n");}
    debug.on = true;
    debug.colorPoint = (SDL_Color){246, 202, 124, 255};
    debug.colorLine = (SDL_Color){158, 189, 127, 255};
    debug.colorFace = (SDL_Color){241, 155, 153, 255};
    debug.colorText = (SDL_Color){116, 173, 220, 255};
    debug.colorDark = (SDL_Color){0, 0, 0, 255};
    debug.colorLight = (SDL_Color){255, 255, 255, 255};
};


// debug functions
void DEBUG_Intro(SDL_Renderer *renderer) {
    if (!debug.on) {return;}

    const Uint64 T1 = 1000, T2 = 2000, T3 = 2500, T4 = 3000;
    static SDL_Texture *textureDark = NULL, *textureLight = NULL;
    static float darkWidth, darkHeight, lightWidth, lightHeight;

    const Uint64 nowTime = SDL_GetTicks();
    if (nowTime < T4) {
        if (textureDark == NULL) {
            textureDark = TXT_LoadTexture(renderer, debug.fontSmall, "[DEBUG:]", debug.colorDark);
            SDL_GetTextureSize(textureDark, &darkWidth, &darkHeight);
        }
        if (textureLight == NULL) {
            textureLight = TXT_LoadTexture(renderer, debug.fontBig, "ON", debug.colorLight);
            SDL_GetTextureSize(textureLight, &lightWidth, &lightHeight);
        }
    }
    else {
        SDL_DestroyTexture(textureLight);
        SDL_DestroyTexture(textureDark);
        textureLight = NULL;
        textureDark = NULL;
    }
    if (T1 <= nowTime && nowTime < T4) {
        const SDL_Color colors[] = {debug.colorPoint, debug.colorLine, debug.colorFace, debug.colorText};
        const int num_colors = sizeof(colors) / sizeof(SDL_Color);
        SDL_FRect rect = {(windowWidth - darkWidth) / 2, (windowHeight - darkHeight) / 2, 0, darkHeight};
        for (int i = 0; i < num_colors; i++) {
            rect.w = darkWidth * (4 - i) / num_colors * EASE_bySin((float)(nowTime - T1) / (T2 - T1));
            SDL_SetRenderSDLColor(renderer, colors[3 - i]);
            SDL_RenderFillRect(renderer, &rect);
        }
        rect.w = darkWidth;
        SDL_RenderTexture(renderer, textureDark, NULL, &rect);
    }
    if (T1 <= nowTime && nowTime < T4) {
        SDL_FRect src_rect = {0, 0, lightWidth, 0};
        SDL_FRect dst_rect = {(windowWidth - lightWidth) / 2, (windowHeight - lightHeight) / 2, lightWidth, 0};
        src_rect.h = dst_rect.h = lightHeight * EASE_bySin((float)(nowTime - T1) / (T3 - T1));
        dst_rect.y += lightHeight - dst_rect.h;
        SDL_RenderTexture(renderer, textureLight, &src_rect, &dst_rect);
    }
}
void DEBUG_DrawPoint(SDL_Renderer* renderer, const Sint16 x, const Sint16 y) {
    if (!debug.on) {
        return;
    }
    if (renderer == NULL) {
        printf("DEBUG_DrawPoint: Renderer is NULL.\n");
        return;
    }

    const float w = 4;
    const SDL_FRect rect = {x - w, y - w, 2 * w, 2 * w};
    SDL_SetRenderDrawColor(renderer, debug.colorPoint.r, debug.colorPoint.g, debug.colorPoint.b, debug.colorPoint.a);
    SDL_RenderFillRect(renderer, &rect);
}
void DEBUG_DrawRect(SDL_Renderer* renderer, const SDL_FRect* rect) {
    if (!debug.on) {
        return;
    }
    if (renderer == NULL) {
        printf("DEBUG_DrawRect: Renderer is NULL.\n");
        return;
    }
    if (rect == NULL) {
        printf("DEBUG_DrawRect: Rect is NULL.\n");
        return;
    }

    SDL_SetRenderDrawColor(renderer, debug.colorFace.r, debug.colorFace.g, debug.colorFace.b, debug.colorFace.a);
    SDL_RenderRect(renderer, rect);
}
void DEBUG_FillRect(SDL_Renderer* renderer, const SDL_FRect* rect) {
    if (!debug.on) {
        return;
    }
    if (renderer == NULL) {
        printf("DEBUG_DrawRect: Renderer is NULL.\n");
        return;
    }
    if (rect == NULL) {
        printf("DEBUG_DrawRect: Rect is NULL.\n");
        return;
    }

    SDL_SetRenderDrawColor(renderer, debug.colorFace.r, debug.colorFace.g, debug.colorFace.b, debug.colorFace.a);
    SDL_RenderFillRect(renderer, rect);
}


#endif //DEBUG_H
