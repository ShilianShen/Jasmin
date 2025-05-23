#ifndef DEBUG_H
#define DEBUG_H


// datatype
struct Debug {
    bool on;
    SDL_Renderer* renderer;
    struct {
        TTF_Font *fontSmall, *fontBig;
        SDL_Color point, line, face, text, dark, light;
    } theme;
};
typedef struct Debug Debug;
Debug debug;


// init
void initDebug() {
    debug = (Debug){0};
}


// load
void loadDebugTheme() {
    debug.theme.fontSmall = TTF_OpenFont("../fonts/JetBrainsMono-ExtraBold.ttf", 16);
    debug.theme.fontBig = TTF_OpenFont("../fonts/JetBrainsMono-ExtraBold.ttf", 32);
    if (debug.theme.fontSmall == NULL) {printf("Fail to load font.\n");}
    debug.on = true;
    debug.theme.point = (SDL_Color){246, 202, 124, 255};
    debug.theme.line = (SDL_Color){158, 189, 127, 255};
    // debug.theme.face = (SDL_Color){241, 155, 153, 255};
    debug.theme.face = (SDL_Color){158, 189, 127, 127};
    debug.theme.text = (SDL_Color){116, 173, 220, 255};
    debug.theme.dark = (SDL_Color){0, 0, 0, 255};
    debug.theme.light = (SDL_Color){255, 255, 255, 255};
};


// debug functions
void DEBUG_Intro() {
    if (!debug.on) {return;}

    const Uint64 T1 = 1000, T2 = 2000, T3 = 2500, T4 = 3000;
    static SDL_Texture *textureDark = NULL, *textureLight = NULL;
    static float darkWidth, darkHeight, lightWidth, lightHeight;

    const Uint64 nowTime = SDL_GetTicks();
    if (nowTime < T4) {
        if (textureDark == NULL) {
            textureDark = TXT_LoadTexture(debug.renderer, debug.theme.fontSmall, "[DEBUG:]", debug.theme.dark);
            SDL_GetTextureSize(textureDark, &darkWidth, &darkHeight);
        }
        if (textureLight == NULL) {
            textureLight = TXT_LoadTexture(debug.renderer, debug.theme.fontBig, "ON", debug.theme.light);
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
        const SDL_Color colors[] = {debug.theme.point, debug.theme.line, debug.theme.face, debug.theme.text};
        const int num_colors = sizeof(colors) / sizeof(SDL_Color);
        SDL_FRect rect = {(windowWidth - darkWidth) / 2, (windowHeight - darkHeight) / 2, 0, darkHeight};
        for (int i = 0; i < num_colors; i++) {
            rect.w = darkWidth * (4 - i) / num_colors * EASE_bySin((float)(nowTime - T1) / (T2 - T1));
            SDL_SetRenderSDLColor(debug.renderer, colors[3 - i]);
            SDL_RenderFillRect(debug.renderer, &rect);
        }
        rect.w = darkWidth;
        SDL_RenderTexture(debug.renderer, textureDark, NULL, &rect);
    }
    if (T1 <= nowTime && nowTime < T4) {
        SDL_FRect src_rect = {0, 0, lightWidth, 0};
        SDL_FRect dst_rect = {(windowWidth - lightWidth) / 2, (windowHeight - lightHeight) / 2, lightWidth, 0};
        src_rect.h = dst_rect.h = lightHeight * EASE_bySin((float)(nowTime - T1) / (T3 - T1));
        dst_rect.y += lightHeight - dst_rect.h;
        SDL_RenderTexture(debug.renderer, textureLight, &src_rect, &dst_rect);
    }
}
void DEBUG_DrawPoint(const Sint16 x, const Sint16 y) {
    // Pre Condition
    if (!debug.on) {return;}

    //
    const float w = 8;
    const SDL_FRect rect = {x - w, y - w, 2 * w, 2 * w};
    SDL_SetRenderSDLColor(debug.renderer, debug.theme.point);
    SDL_RenderFillRect(debug.renderer, &rect);
}
void DEBUG_DrawRect(const SDL_FRect* rect) {
    // Pre Condition
    if (!debug.on) {return;}

    // Req Condition
    if (rect == NULL) {printf("%s: rect is NULL.\n", __func__); return;}

    //
    SDL_SetRenderSDLColor(debug.renderer, debug.theme.line);
    SDL_RenderRect(debug.renderer, rect);
}
void DEBUG_FillRect(const SDL_FRect* rect) {
    // Pre Condition
    if (!debug.on) {return;}

    // Req Condition
    if (rect == NULL) {printf("%s: rect is NULL.\n", __func__); return;}

    //
    SDL_SetRenderSDLColor(debug.renderer, debug.theme.face);
    SDL_RenderFillRect(debug.renderer, rect);
}
void DEBUG_DrawText(const Sint16 x, const Sint16 y, const char* text) {
    // Pre Condition
    if (!debug.on) {return;}

    // Req Condition
    if (text == NULL) {printf("%s: text is NULL.\n", __func__); return;}

    //
    SDL_Texture* textTexture = TXT_LoadTexture(debug.renderer, debug.theme.fontSmall, text, debug.theme.text);
    if (textTexture == NULL) {printf("%s: texture is NULL.\n", __func__); return;}
    SDL_FRect dst_rect = {x, y};
    SDL_GetTextureSize(textTexture, &dst_rect.w, &dst_rect.h);
    SDL_RenderTexture(debug.renderer, textTexture, NULL, &dst_rect);
    SDL_DestroyTexture(textTexture);
}


#endif //DEBUG_H
