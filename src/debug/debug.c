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


const char* DEBUG_JSON = "../config/debug.json";
const int MESSAGE_SIZE_MAX = 128;
const int DETAIL_SIZE_MAX = 64;
const bool DEBUG_ON = true;


struct {
    cJSON* json;
    TTF_Font *font;
    SDL_Color colors[DEBUG_NUM_COLORS][DEBUG_NUM_ALPHAS];
    char* message[2];
} debug;


// GET & SET ===========================================================================================================
bool DEBUG_GetShift() {
    if (!DEBUG_ON) return false;
    return SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LSHIFT];
}



// INIT & EXIT =========================================================================================================
bool DEBUG_Init() {
    if (!DEBUG_ON) return true;

    memset(&debug, 0, sizeof(debug));

    debug.json = getJson(DEBUG_JSON);
    REQ_CONDITION(debug.json != NULL, return false);
    REQ_CONDITION(cJSON_IsObject(debug.json), return false);

    char* font_path = NULL;
    float font_size = 0;
    REQ_CONDITION(cJSON_LoadByKey(debug.json, "font_path", JSM_STRING, &font_path), return false);
    REQ_CONDITION(cJSON_LoadByKey(debug.json, "font_size", JSM_FLOAT, &font_size), return false);

    debug.font = BASIC_GetFont("debug");
    REQ_CONDITION(debug.font != NULL, return false);

    SDL_Color colors[DEBUG_NUM_COLORS];
    REQ_CONDITION(cJSON_LoadByKey(debug.json, "color_point", JSM_COLOR, &colors[DEBUG_COLOR_POINT]), return false);
    REQ_CONDITION(cJSON_LoadByKey(debug.json, "color_rect" , JSM_COLOR, &colors[DEBUG_COLOR_RECT ]), return false);
    REQ_CONDITION(cJSON_LoadByKey(debug.json, "color_face" , JSM_COLOR, &colors[DEBUG_COLOR_FACE ]), return false);
    REQ_CONDITION(cJSON_LoadByKey(debug.json, "color_text" , JSM_COLOR, &colors[DEBUG_COLOR_TEXT ]), return false);
    REQ_CONDITION(cJSON_LoadByKey(debug.json, "color_dark" , JSM_COLOR, &colors[DEBUG_COLOR_DARK ]), return false);
    REQ_CONDITION(cJSON_LoadByKey(debug.json, "color_light", JSM_COLOR, &colors[DEBUG_COLOR_LIGHT]), return false);

    int alphas[DEBUG_NUM_ALPHAS];
    REQ_CONDITION(cJSON_LoadByKey(debug.json, "alpha_dark" , JSM_INT, &alphas[DEBUG_ALPHA_DARK ]), return false);
    REQ_CONDITION(cJSON_LoadByKey(debug.json, "alpha_light", JSM_INT, &alphas[DEBUG_ALPHA_LIGHT]), return false);

    for (int i = 0; i < DEBUG_NUM_COLORS; i++) {
        for (int j = 0; j < DEBUG_NUM_ALPHAS; j++) {
            debug.colors[i][j] = colors[i];
            debug.colors[i][j].a = alphas[j];
        }
    }

    cJSON_Delete(debug.json);
    debug.json = NULL;

    return true;
}
void DEBUG_Exit() {
    if (debug.json != NULL) {
        cJSON_Delete(debug.json);
        debug.json = NULL;
    }
}


// RENEW ===============================================================================================================
static void DEBUG_Renew_Fps() {
    static float fpsSet[1000];
    static int head = 0;

    if (BASIC_DT != 0) {
        const float fps = 1.f / BASIC_DT;
        fpsSet[head] = fps;
        head = (head + 1) % len_of(fpsSet);
    }
    float min = 100000, max = -1;
    for (int i = 0; i < len_of(fpsSet); i++) {
        min = SDL_min(min, fpsSet[i]);
        max = SDL_max(max, fpsSet[i]);
    }
    DEBUG_SendMessageL("%s:\n", __func__);
    DEBUG_SendMessageL("    min: %4.2f\n", min);
    DEBUG_SendMessageL("    max: %4.2f\n", max);
}
bool DEBUG_Renew() {
    if (!DEBUG_ON) return true;

    for (int i = 0; i < 2; i++) {
        if (debug.message[i] != NULL) {
            free(debug.message[i]);
            debug.message[i] = NULL;
        }
    }
    DEBUG_Renew_Fps();

    return true;
}


// DRAW ================================================================================================================
bool DEBUG_Draw() {
    if (!DEBUG_ON) return true;
    if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LSHIFT] == false) return true;

    // Opt Condition
    if (debug.message[0] != NULL) {DEBUG_DrawTextAligned(debug.message[0], 'L');}
    if (debug.message[1] != NULL) {DEBUG_DrawTextAligned(debug.message[1], 'R');}
    return true;
}


void DEBUG_DrawPoint(const SDL_FPoint point) {
    if (!DEBUG_ON) return;

    const float w = 8;
    const SDL_FRect rect = {point.x - w, point.y - w, 2 * w, 2 * w};

    SDL_SetRenderColor(renderer, debug.colors[DEBUG_COLOR_POINT][DEBUG_ALPHA_LIGHT]);
    SDL_RenderFillRect(renderer, &rect);
}
void DEBUG_DrawLine(const SDL_FPoint point1, const SDL_FPoint point2) {
    if (!DEBUG_ON) return;

    SDL_SetRenderColor(renderer, debug.colors[DEBUG_COLOR_POINT][DEBUG_ALPHA_LIGHT]);
    SDL_RenderLine(renderer, point1.x, point1.y, point2.x, point2.y);
}
void DEBUG_DrawRect(const SDL_FRect rect) {
    if (!DEBUG_ON) return;

    SDL_SetRenderColor(renderer, debug.colors[DEBUG_COLOR_RECT][DEBUG_ALPHA_LIGHT]);
    SDL_RenderRect(renderer, &rect);
}
void DEBUG_FillRect(const SDL_FRect rect) {
    if (!DEBUG_ON) return;

    SDL_SetRenderColor(renderer, debug.colors[DEBUG_COLOR_RECT][DEBUG_ALPHA_DARK]);
    SDL_RenderFillRect(renderer, &rect);

    SDL_SetRenderColor(renderer, debug.colors[DEBUG_COLOR_RECT][DEBUG_ALPHA_LIGHT]);
    SDL_RenderRect(renderer, &rect);
}
static SDL_Texture* DEBUG_GetTextTexture(const char* text, const char aligned) {
    SDL_Texture* textTexture = TXT_LoadTextureWithLines(
        renderer,
        debug.font,
        text,
        debug.colors[DEBUG_COLOR_LIGHT][DEBUG_ALPHA_LIGHT],
        debug.colors[DEBUG_COLOR_DARK][DEBUG_ALPHA_DARK],
        aligned
        );
    REQ_CONDITION(textTexture != NULL, return NULL);
    return textTexture;
}
void DEBUG_DrawText(const SDL_FPoint point, const char* text) {
    if (!DEBUG_ON) return;

    REQ_CONDITION(text != NULL, return);

    SDL_Texture* textTexture = DEBUG_GetTextTexture(text, 'L');
    REQ_CONDITION(textTexture != NULL, return);

    SDL_FRect dst_rect = {point.x, point.y};
    SDL_GetTextureSize(textTexture, &dst_rect.w, &dst_rect.h);
    SDL_RenderTexture(renderer, textTexture, NULL, &dst_rect);
    SDL_DestroyTexture(textTexture);
}
void DEBUG_DrawTextAligned(const char* text, const char aligned) {
    if (!DEBUG_ON) return;

    REQ_CONDITION(text != NULL, return);

    SDL_Texture* textTexture = DEBUG_GetTextTexture(text, aligned);
    REQ_CONDITION(textTexture != NULL, return);

    const int anchor = aligned == 'R' ? 20 : -20;
    SDL_RenderTextureAligned(renderer, textTexture, NULL, NULL, NULL, anchor);
    SDL_DestroyTexture(textTexture);
}


void DEBUG_DrawGeometry(
    SDL_Renderer *renderer,
    SDL_Texture* texture,
    const SDL_Vertex *vertices,
    const int num_vertices,
    const int *indices,
    const int num_indices
    ) {
    if (!DEBUG_ON) return;

    SDL_Vertex debugVertices[num_vertices];
    for (int i = 0; i < num_vertices; i++) {
        debugVertices[i] = vertices[i];
        debugVertices[i].color = SDL_GetFColorFromColor(debug.colors[DEBUG_COLOR_FACE][DEBUG_ALPHA_DARK]);
    }
    SDL_RenderGeometry(renderer, texture, debugVertices, num_vertices, indices, num_indices);
    SDL_SetRenderColor(renderer, debug.colors[DEBUG_COLOR_FACE][DEBUG_ALPHA_LIGHT]);
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


// MESSAGE =============================================================================================================
static void DEBUG_SendMessage(const int i, const char* newMessage) {
    if (!DEBUG_ON) return;

    // getMessage
    if (debug.message[i] == NULL) {
        debug.message[i] = strdup(newMessage); // malloc
        REQ_CONDITION(debug.message[i] != NULL, return);
    }
    else {
        // getOldMessage
        char* oldMessage = debug.message[i]; // malloc

        // getAllMessage
        debug.message[i] = malloc(strlen(newMessage) + strlen(oldMessage) + 1); // malloc
        REQ_CONDITION(debug.message[i] != NULL, );

        strcpy(debug.message[i], oldMessage);
        strcat(debug.message[i], newMessage);
        free(oldMessage); // free
    }
}
void DEBUG_SendMessageL(const char* format, ...) {
    if (!DEBUG_ON) return;

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
    if (!DEBUG_ON) return;

    // getNewMessage
    char newMessage[MESSAGE_SIZE_MAX] = ""; // not malloc
    va_list args;
    va_start(args, format);
    vsnprintf(newMessage, MESSAGE_SIZE_MAX, format, args);
    va_end(args);

    //
    DEBUG_SendMessage(1, newMessage);
}