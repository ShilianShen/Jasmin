#include "debug.h"


typedef struct Debug Debug;
struct Debug {
    SDL_Renderer* renderer;
    struct {
        TTF_Font *font;
        SDL_Color point, rect, face, text, dark, light;
        SDL_Color darkPoint, darkRect, darkFace, darkText;
        int alphaLight, alphaDark;
    } theme;
    char* message[2];  // malloc or NULL
} debug;
const int MESSAGE_SIZE_MAX = 128;
const int DETAIL_SIZE_MAX = 64;


static void DEBUG_LoadTheme() {
    debug.theme.font = TTF_OpenFont("../res/font/JetBrainsMono-Regular.ttf", 24);

    // color
    debug.theme.point = (SDL_Color){246, 202, 124, 255};
    debug.theme.rect = (SDL_Color){241, 155, 153, 255};
    debug.theme.face = (SDL_Color){158, 189, 127, 255};
    debug.theme.text = (SDL_Color){116, 173, 220, 255};
    debug.theme.dark = (SDL_Color){0, 0, 0, 128};
    debug.theme.light = (SDL_Color){255, 255, 255, 255};

    // alpha
    debug.theme.alphaLight = 255;
    debug.theme.alphaDark = 64;

    debug.theme.darkPoint = debug.theme.point;
    debug.theme.darkRect = debug.theme.rect;
    debug.theme.darkFace = debug.theme.face;
    debug.theme.darkText = debug.theme.text;

    debug.theme.darkPoint.a = debug.theme.alphaDark;
    debug.theme.darkRect.a = debug.theme.alphaDark;
    debug.theme.darkFace.a = debug.theme.alphaDark;
    debug.theme.darkText.a = debug.theme.alphaDark;
}
static void DEBUG_Load() {
    DEBUG_LoadTheme();
}
bool DEBUG_Init() {
    // Req Condition
    if (renderer == NULL) {printf("%s: renderer not exists.\n", __func__); return false;}

    //
    debug = (Debug){0};
    debug.renderer = renderer;
    // 设置渲染器的混合模式（启用 Alpha 混合）
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    DEBUG_Load();
    return true;
}
bool DEBUG_Renew() {
    // Pre Condition
    if (!DEBUG_ON) {return true;}

    //
    for (int i = 0; i < 2; i++) {
        if (debug.message[i] != NULL) {
            free(debug.message[i]);
            debug.message[i] = NULL;
        }
    }

    //
    static Uint64 t1 = 0, t2 = 0;
    t2 = SDL_GetTicks();
    DEBUG_SendMessageL("FPS: %4.2f\n", 1000.f / (float)(t2 - t1));
    t1 = SDL_GetTicks();
    return true;
}
void DEBUG_Exit() {
    TTF_CloseFont(debug.theme.font);
}


void DEBUG_DrawPoint(const SDL_FPoint point) {
    if (!DEBUG_ON) return; // Pre Condition

    const float w = 8;
    const SDL_FRect rect = {point.x - w, point.y - w, 2 * w, 2 * w};

    SDL_SetRenderSDLColor(debug.renderer, debug.theme.point);
    SDL_RenderFillRect(debug.renderer, &rect);
}
void DEBUG_DrawLine(const SDL_FPoint point1, const SDL_FPoint point2) {
    if (!DEBUG_ON) return; // Pre Condition

    SDL_SetRenderSDLColor(debug.renderer, debug.theme.point);
    SDL_RenderLine(debug.renderer, point1.x, point1.y, point2.x, point2.y);
}
void DEBUG_DrawRect(const SDL_FRect* rect) {
    if (!DEBUG_ON) return; // Pre Condition

    // Req Condition
    if (rect == NULL) {printf("%s: rect is NULL.\n", __func__); return;}

    // rect edge
    SDL_SetRenderSDLColor(debug.renderer, debug.theme.rect);
    SDL_RenderRect(debug.renderer, rect);
}
void DEBUG_FillRect(const SDL_FRect* rect) {
    // Pre Condition
    if (!DEBUG_ON) {return;}

    // Req Condition
    if (rect == NULL) {printf("%s: rect is NULL.\n", __func__); return;}

    // rect back
    SDL_SetRenderSDLColor(debug.renderer, debug.theme.darkRect);
    SDL_RenderFillRect(debug.renderer, rect);

    // rect edge
    SDL_SetRenderSDLColor(debug.renderer, debug.theme.rect);
    SDL_RenderRect(debug.renderer, rect);
}
static SDL_Texture* DEBUG_GetTextTexture(const char* text, const char aligned) {
    // Pre Condition
    if (!DEBUG_ON) {return NULL;}

    // Req Condition
    if (text == NULL) {printf("%s: text is NULL.\n", __func__); return NULL;}

    // Req Condition
    SDL_Texture* textTexture = TXT_LoadTextureWithLines(
        debug.renderer, debug.theme.font, text, debug.theme.light, debug.theme.dark, aligned
        );
    if (textTexture == NULL) {printf("%s: texture is NULL.\n", __func__); return NULL;}

    return textTexture;
}
void DEBUG_DrawText(const Sint16 x, const Sint16 y, const char* text) {
    // Pre Condition
    if (!DEBUG_ON) {return;}

    // Req Condition
    if (text == NULL) {printf("%s: text is NULL.\n", __func__); return;}

    // Req Condition
    SDL_Texture* textTexture = DEBUG_GetTextTexture(text, 'L');
    if (textTexture == NULL) {printf("%s: texture is NULL.\n", __func__); return;}

    // rect
    SDL_FRect dst_rect = {x, y};
    SDL_GetTextureSize(textTexture, &dst_rect.w, &dst_rect.h);

    // text
    SDL_RenderTexture(debug.renderer, textTexture, NULL, &dst_rect);
    SDL_DestroyTexture(textTexture);
}
void DEBUG_DrawTextAligned(const char* text, const char aligned) {
    // Pre Condition
    if (!DEBUG_ON) {return;}

    // Req Condition
    if (text == NULL) {printf("%s: text is NULL.\n", __func__); return;}

    // Req Condition
    SDL_Texture* textTexture = DEBUG_GetTextTexture(text, aligned);
    if (textTexture == NULL) {printf("%s: texture is NULL.\n", __func__); return;}

    //
    const int anchor = aligned == 'R' ? 20 : -20;
    SDL_RenderTextureAligned(debug.renderer, textTexture, NULL, NULL, NULL, anchor);
    SDL_DestroyTexture(textTexture);
}

void DEBUG_DrawGeometry(
    SDL_Renderer *renderer,
    SDL_Texture *texture,
    const SDL_Vertex *vertices,
    const int num_vertices,
    const int *indices,
    const int num_indices) {
    if (DEBUG_ON == false) {
        return;
    }

    SDL_Vertex debugVertices[num_vertices];
    for (int i = 0; i < num_vertices; i++) {
        debugVertices[i] = vertices[i];
        debugVertices[i].color = SDL_GetFColorFromColor(debug.theme.darkFace);
    }
    SDL_RenderGeometry(renderer, NULL, debugVertices, num_vertices, indices, num_indices);
    SDL_SetRenderSDLColor(debug.renderer, debug.theme.face);
    for (int i = 0; i < num_indices; i += 3) {
        const SDL_FPoint points[4] = {
            debugVertices[indices[i]].position,
            debugVertices[indices[i+1]].position,
            debugVertices[indices[i+2]].position,
            debugVertices[indices[i]].position,
        };
        SDL_RenderLines(renderer, points, 4);
    }
}


static void DEBUG_SendMessage(const int i, const char* newMessage) {
    // Pre Condition
    if (!DEBUG_ON) {return;}

    // getMessage
    if (debug.message[i] == NULL) {
        debug.message[i] = strdup(newMessage); // malloc
        if (debug.message[i] == NULL) {
            printf("%s: failed to malloc.\n", __func__);
        }
    }
    else {
        // getOldMessage
        char* oldMessage = debug.message[i]; // malloc

        // getAllMessage
        debug.message[i] = malloc(strlen(newMessage) + strlen(oldMessage) + 1); // malloc
        if (debug.message[i] == NULL) {
            printf("%s: failed to malloc.\n", __func__);
        }
        strcpy(debug.message[i], oldMessage);
        strcat(debug.message[i], newMessage);
        free(oldMessage); // free
    }
}
void DEBUG_SendMessageL(const char* format, ...) {
    // Pre Condition
    if (!DEBUG_ON) {return;}

    // getNewMessage
    char newMessage[MESSAGE_SIZE_MAX] = ""; // not malloc
    va_list args;
    va_start(args, format);
    vsnprintf(newMessage, MESSAGE_SIZE_MAX, format, args);
    va_end(args);

    //
    DEBUG_SendMessage(0, newMessage);
}
void DEBUG_SendMessageR(const char* format, ...) {
    // Pre Condition
    if (!DEBUG_ON) {return;}

    // getNewMessage
    char newMessage[MESSAGE_SIZE_MAX] = ""; // not malloc
    va_list args;
    va_start(args, format);
    vsnprintf(newMessage, MESSAGE_SIZE_MAX, format, args);
    va_end(args);

    //
    DEBUG_SendMessage(1, newMessage);
}

bool DEBUG_Draw() {
    // Pre Condition
    if (!DEBUG_ON) {return true;}
    const bool* state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_LSHIFT] == false) { return true; }

    // Opt Condition
    if (debug.message[0] != NULL) {DEBUG_DrawTextAligned(debug.message[0], 'L');}
    if (debug.message[1] != NULL) {DEBUG_DrawTextAligned(debug.message[1], 'R');}
    return true;
}
