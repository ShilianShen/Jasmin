#ifndef DEBUG_H
#define DEBUG_H


// datatype
struct Debug {
    bool on;
    SDL_Renderer* renderer;
    struct {
        TTF_Font *font32, *font64, *font128, *font256;
        SDL_Color colorPoint, colorRect, colorFace, colorText, colorDark, colorLight;
        int alphaLight, alphaDark;
    } theme;
    char message[50];
};
typedef struct Debug Debug;
Debug debug;


// init
void DEBUG_Init(SDL_Renderer* renderer) {
    // Req Condition
    if (renderer == NULL) {printf("%s: renderer not exists.\n", __func__); return;}

    //
    debug = (Debug){0};
    debug.renderer = renderer;
}


// load
static void DEBUG_LoadTheme() {
    debug.theme.font32 = TTF_OpenFont("../fonts/JetBrainsMono-Regular.ttf", 32);
    debug.theme.font64 = TTF_OpenFont("../fonts/JetBrainsMono-Regular.ttf", 64);
    debug.theme.font128 = TTF_OpenFont("../fonts/JetBrainsMono-Regular.ttf", 128);
    debug.theme.font256 = TTF_OpenFont("../fonts/JetBrainsMono-Regular.ttf", 256);
    if (debug.theme.font128 == NULL) {printf("Fail to load font.\n");}
    debug.on = true;

    // color
    debug.theme.colorPoint = (SDL_Color){246, 202, 124, 255};
    debug.theme.colorRect = (SDL_Color){241, 155, 153, 255};
    debug.theme.colorFace = (SDL_Color){158, 189, 127, 255};
    debug.theme.colorText = (SDL_Color){116, 173, 220, 255};
    debug.theme.colorDark = (SDL_Color){0, 0, 0, 255};
    debug.theme.colorLight = (SDL_Color){255, 255, 255, 255};

    // alpha
    debug.theme.alphaLight = 255;
    debug.theme.alphaDark = 128;
}
void DEBUG_Load() {
    DEBUG_LoadTheme();
}


// renew
void DEBUG_Renew() {
    // Pre Condition
    if (!debug.on) {return;}

    //
    debug.message[0] = '\0';
}


// debug functions
void DEBUG_Intro() {
    // Pre Condition
    if (!debug.on) {return;}

    static const Uint64 T1 = 1000, T2 = 2000, T3 = 2500, T4 = 3000;
    static SDL_Texture *textureDark = NULL, *textureLight = NULL;
    static float darkWidth, darkHeight, lightWidth, lightHeight;

    const Uint64 nowTime = SDL_GetTicks();
    if (nowTime < T4) {
        if (textureDark == NULL) {
            textureDark = TXT_LoadTexture(debug.renderer, debug.theme.font128, "[DEBUG:]", debug.theme.colorDark);
            SDL_GetTextureSize(textureDark, &darkWidth, &darkHeight);
        }
        if (textureLight == NULL) {
            textureLight = TXT_LoadTexture(debug.renderer, debug.theme.font256, "ON", debug.theme.colorLight);
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
        const SDL_Color colors[] = {debug.theme.colorPoint, debug.theme.colorRect, debug.theme.colorFace, debug.theme.colorText};
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

    // point
    const float w = 8;
    const SDL_FRect rect = {x - w, y - w, 2 * w, 2 * w};

    // draw
    SDL_SetRenderSDLColorAlpha(debug.renderer, debug.theme.colorPoint, debug.theme.alphaLight);
    SDL_RenderFillRect(debug.renderer, &rect);
}
void DEBUG_DrawRect(const SDL_FRect* rect) {
    // Pre Condition
    if (!debug.on) {return;}

    // Req Condition
    if (rect == NULL) {printf("%s: rect is NULL.\n", __func__); return;}

    // rect edge
    SDL_SetRenderSDLColorAlpha(debug.renderer, debug.theme.colorRect, debug.theme.alphaLight);
    SDL_RenderRect(debug.renderer, rect);
}
void DEBUG_FillRect(const SDL_FRect* rect) {
    // Pre Condition
    if (!debug.on) {return;}

    // Req Condition
    if (rect == NULL) {printf("%s: rect is NULL.\n", __func__); return;}

    // rect back
    SDL_SetRenderSDLColorAlpha(debug.renderer, debug.theme.colorRect, debug.theme.alphaDark);
    SDL_RenderFillRect(debug.renderer, rect);

    // rect edge
    SDL_SetRenderSDLColorAlpha(debug.renderer, debug.theme.colorRect, debug.theme.alphaLight);
    SDL_RenderRect(debug.renderer, rect);
}
void DEBUG_DrawText(const Sint16 x, const Sint16 y, const char* text) {
    // Pre Condition
    if (!debug.on) {return;}

    // Req Condition
    if (text == NULL) {printf("%s: text is NULL.\n", __func__); return;}

    // Req Condition
    SDL_Texture* textTexture = TXT_LoadTextureWithLines(
        debug.renderer, debug.theme.font32, text, debug.theme.colorLight, 'L'
        );
    if (textTexture == NULL) {printf("%s: texture is NULL.\n", __func__); return;}

    // rect
    SDL_FRect dst_rect = {x, y};
    SDL_GetTextureSize(textTexture, &dst_rect.w, &dst_rect.h);

    // rect back
    SDL_SetRenderSDLColorAlpha(debug.renderer, debug.theme.colorText, debug.theme.alphaDark);
    SDL_RenderFillRect(debug.renderer, &dst_rect);

    // rect edge
    SDL_SetRenderSDLColorAlpha(debug.renderer, debug.theme.colorText, debug.theme.alphaLight);
    SDL_RenderRect(debug.renderer, &dst_rect);

    // text
    SDL_RenderTexture(debug.renderer, textTexture, NULL, &dst_rect);
    SDL_DestroyTexture(textTexture);
}
void DEBUG_SendMessage(const char* format, ...) {
    // Pre Condition
    if (!debug.on) {return;}

    //
    va_list args;
    va_start(args, format);
    vsnprintf(debug.message, 50, format, args);
    va_end(args);
}


#endif //DEBUG_H
