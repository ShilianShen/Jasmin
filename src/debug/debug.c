#include "debug.h"



// PARA ================================================================================================================
enum DEBUG_COLOR {
    DEBUG_COLOR_POINT,
    DEBUG_COLOR_RECT,
    DEBUG_COLOR_FACE,
    DEBUG_COLOR_TEXT,
    DEBUG_COLOR_DARK,
    DEBUG_COLOR_LIGHT,
    DEBUG_NUM_COLORS,
};
enum DEBUG_ALPHA {
    DEBUG_ALPHA_DARK,
    DEBUG_ALPHA_LIGHT,
    DEBUG_NUM_ALPHAS,
};

const char* DEBUG_JSON = "../config/debug_theme.json";


typedef struct Debug Debug;
struct Debug {
    TTF_Font *font;
    SDL_Color colors[DEBUG_NUM_COLORS][DEBUG_NUM_ALPHAS];
    struct {
        TTF_Font *font;
    } theme;
    char* message[2];
};
static Debug debug;
const int MESSAGE_SIZE_MAX = 128;
const int DETAIL_SIZE_MAX = 64;


const bool DEBUG_ON = true;
static void DEBUG_II_RK(const cJSON* debug_json) {
    if (cJSON_IsObject(debug_json) == false) {
        printf("%s: cJSON_IsObject(debug_json).\n", __func__);
        return;
    }

    char* key = NULL;
    char* font_path = NULL;
    float font_size = 0;
    if (cJSON_LoadFromObj(debug_json, key = "font_path", JSM_STRING, &font_path) == false) {
        printf("%s: cJSON_LoadFromObj failed.\n", __func__);
        return;
    }
    if (cJSON_LoadFromObj(debug_json, key = "font_size", JSM_FLOAT, &font_size) == false) {
        printf("%s: cJSON_LoadFromObj failed.\n", __func__);
        return;
    }

    SDL_Color colors[DEBUG_NUM_COLORS];
    if (cJSON_LoadFromObj(debug_json, key = "color_point", JSM_COLOR, &colors[DEBUG_COLOR_POINT]) == false) {
        printf("%s: cJSON_LoadFromObj failed.\n", __func__);
        return;
    }
    if (cJSON_LoadFromObj(debug_json, key = "color_rect", JSM_COLOR, &colors[DEBUG_COLOR_RECT]) == false) {
        printf("%s: cJSON_LoadFromObj failed.\n", __func__);
        return;
    }
    if (cJSON_LoadFromObj(debug_json, key = "color_face", JSM_COLOR, &colors[DEBUG_COLOR_FACE]) == false) {
        printf("%s: cJSON_LoadFromObj failed.\n", __func__);
        return;
    }
    if (cJSON_LoadFromObj(debug_json, key = "color_text", JSM_COLOR, &colors[DEBUG_COLOR_TEXT]) == false) {
        printf("%s: cJSON_LoadFromObj failed.\n", __func__);
        return;
    }
    if (cJSON_LoadFromObj(debug_json, key = "color_dark", JSM_COLOR, &colors[DEBUG_COLOR_DARK]) == false) {
        printf("%s: cJSON_LoadFromObj failed.\n", __func__);
        return;
    }
    if (cJSON_LoadFromObj(debug_json, key = "color_light", JSM_COLOR, &colors[DEBUG_COLOR_LIGHT]) == false) {
        printf("%s: cJSON_LoadFromObj failed.\n", __func__);
        return;
    }

    int alphas[DEBUG_NUM_ALPHAS];
    if (cJSON_LoadFromObj(debug_json, key = "alpha_dark", JSM_INT, &alphas[DEBUG_ALPHA_DARK]) == false) {
        printf("%s: cJSON_LoadFromObj failed.\n", __func__);
        return;
    }
    if (cJSON_LoadFromObj(debug_json, key = "alpha_light", JSM_INT, &alphas[DEBUG_ALPHA_LIGHT]) == false) {
        printf("%s: cJSON_LoadFromObj failed.\n", __func__);
        return;
    }

    for (int i = 0; i < DEBUG_NUM_COLORS; i++) {
        for (int j = 0; j < DEBUG_NUM_ALPHAS; j++) {
            debug.colors[i][j] = colors[i];
            debug.colors[i][j].a = alphas[j];
        }
    }
}
static void DEBUG_II() {
    cJSON* debug_json = getJson(DEBUG_JSON);
    if (debug_json == NULL) {
        printf("%s: debug_json == NULL.\n", __func__);
        return;
    }
    DEBUG_II_RK(debug_json);
    cJSON_Delete(debug_json);
}
static void DEBUG_LoadTheme() {
    debug.theme.font = TTF_OpenFont("../res/font/JetBrainsMono-Regular.ttf", 24);
}
static void DEBUG_Load() {
    DEBUG_LoadTheme();
}
bool DEBUG_Init() {
    // Req Condition
    if (renderer == NULL) {printf("%s: renderer not exists.\n", __func__); return false;}

    //
    debug = (Debug){0};
    DEBUG_II();
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


// DRAW ================================================================================================================
void DEBUG_DrawPoint(const SDL_FPoint point) {
    if (!DEBUG_ON) return;

    const float w = 8;
    const SDL_FRect rect = {point.x - w, point.y - w, 2 * w, 2 * w};

    SDL_SetRenderSDLColor(renderer, debug.colors[DEBUG_COLOR_POINT][DEBUG_ALPHA_LIGHT]);
    SDL_RenderFillRect(renderer, &rect);
}
void DEBUG_DrawLine(const SDL_FPoint point1, const SDL_FPoint point2) {
    if (!DEBUG_ON) return;

    SDL_SetRenderSDLColor(renderer, debug.colors[DEBUG_COLOR_POINT][DEBUG_ALPHA_LIGHT]);
    SDL_RenderLine(renderer, point1.x, point1.y, point2.x, point2.y);
}
void DEBUG_DrawRect(const SDL_FRect rect) {
    if (!DEBUG_ON) return;

    SDL_SetRenderSDLColor(renderer, debug.colors[DEBUG_COLOR_RECT][DEBUG_ALPHA_LIGHT]);
    SDL_RenderRect(renderer, &rect);
}
void DEBUG_FillRect(const SDL_FRect rect) {
    if (!DEBUG_ON) return;

    SDL_SetRenderSDLColor(renderer, debug.colors[DEBUG_COLOR_RECT][DEBUG_ALPHA_DARK]);
    SDL_RenderFillRect(renderer, &rect);

    SDL_SetRenderSDLColor(renderer, debug.colors[DEBUG_COLOR_RECT][DEBUG_ALPHA_LIGHT]);
    SDL_RenderRect(renderer, &rect);
}
static SDL_Texture* DEBUG_GetTextTexture(const char* text, const char aligned) {
    if (text == NULL) {
        printf("%s: text == NULL.\n", __func__);
        return NULL;
    }
    SDL_Texture* textTexture = TXT_LoadTextureWithLines(
        renderer,
        debug.theme.font,
        text,
        debug.colors[DEBUG_COLOR_LIGHT][DEBUG_ALPHA_LIGHT],
        debug.colors[DEBUG_COLOR_DARK][DEBUG_ALPHA_DARK],
        aligned
        );
    if (textTexture == NULL) {printf("%s: textTexture == NULL.\n", __func__); return NULL;}

    return textTexture;
}
void DEBUG_DrawText(const Sint16 x, const Sint16 y, const char* text) {
    if (!DEBUG_ON) return;

    // Req Condition
    if (text == NULL) {printf("%s: text is NULL.\n", __func__); return;}

    // Req Condition
    SDL_Texture* textTexture = DEBUG_GetTextTexture(text, 'L');
    if (textTexture == NULL) {printf("%s: texture is NULL.\n", __func__); return;}

    // rect
    SDL_FRect dst_rect = {x, y};
    SDL_GetTextureSize(textTexture, &dst_rect.w, &dst_rect.h);

    // text
    SDL_RenderTexture(renderer, textTexture, NULL, &dst_rect);
    SDL_DestroyTexture(textTexture);
}
void DEBUG_DrawTextAligned(const char* text, const char aligned) {
    if (!DEBUG_ON) return;

    // Req Condition
    if (text == NULL) {printf("%s: text is NULL.\n", __func__); return;}

    // Req Condition
    SDL_Texture* textTexture = DEBUG_GetTextTexture(text, aligned);
    if (textTexture == NULL) {printf("%s: texture is NULL.\n", __func__); return;}

    //
    const int anchor = aligned == 'R' ? 20 : -20;
    SDL_RenderTextureAligned(renderer, textTexture, NULL, NULL, NULL, anchor);
    SDL_DestroyTexture(textTexture);
}


void DEBUG_DrawGeometry(
    SDL_Renderer *renderer,
    SDL_Texture *texture,
    const SDL_Vertex *vertices,
    const int num_vertices,
    const int *indices,
    const int num_indices) {
    if (!DEBUG_ON) return;

    SDL_Vertex debugVertices[num_vertices];
    for (int i = 0; i < num_vertices; i++) {
        debugVertices[i] = vertices[i];
        debugVertices[i].color = SDL_GetFColorFromColor(debug.colors[DEBUG_COLOR_FACE][DEBUG_ALPHA_DARK]);
    }
    SDL_RenderGeometry(renderer, NULL, debugVertices, num_vertices, indices, num_indices);
    SDL_SetRenderSDLColor(renderer, debug.colors[DEBUG_COLOR_FACE][DEBUG_ALPHA_LIGHT]);
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
