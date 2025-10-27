#include "message.h"


#define MESSAGE_FONT "../res/font/IBMPlexMono-Medium.ttf", 48
static TTF_Font* font = NULL;
static float fontHeight = 0.f;
static SDL_FRect back;
static float backSpaceOut = 100, backSpaceIn = 10;
static SDL_Texture* sprite = NULL;
static const float SPRITE_SCALE = 8;


static float startTime = 0.f;
static const float EDGE_WIDTH = 8;
static const SDL_Color EDGE_COLOR = {64, 64, 64, 255};
static const SDL_Color TEXT_COLOR = {100, 100, 100, 255};
static const SDL_Color BACK_COLOR = {225, 225, 225, 255};
static const int CHAR_PER_SEC = 50;
static const float SPRITE_SPEED = 4;


static char* messageName = NULL;
static SDL_Texture* nameTex = NULL;


typedef struct {
    char* string;
    SDL_Texture* texture;
    SDL_FRect dstRect;
    float time;
} Message;
static const int NUM_MESSAGES = 4;
static Message box[NUM_MESSAGES] = {0};


// GET & SET ===========================================================================================================
void VILLA_FreeMessage(Message* message) {
    if (message == NULL) return;
    if (message->string != NULL) {
        free(message->string);
        message->string = NULL;
    }
    if (message->texture != NULL) {
        SDL_DestroyTexture(message->texture);
        message->texture = NULL;
    }
}
bool VILLA_SendMessage(const char* name, const char* string) {
    REQ_CONDITION(name != NULL && string != NULL, return false);

    for (int i = 0; i < NUM_MESSAGES; i++) {
        if (box[i].string != NULL) continue;
        box[i].string = strdup(string);
        REQ_CONDITION(box[i].string != NULL, return false);
        box[i].time = BASIC_T2;
        if (i == 0) {
            startTime = BASIC_T2;
        }
        return true;
    }

    VILLA_FreeMessage(&box[0]);
    for (int i = 1; i < NUM_MESSAGES; i++) {
        box[i - 1] = box[i];
    }
    box[NUM_MESSAGES - 1].string = strdup(string);
    REQ_CONDITION(box[NUM_MESSAGES - 1].string != NULL, return false);
    box[NUM_MESSAGES - 1].time = BASIC_T2;

    if (messageName != NULL) {
        free(messageName);
        messageName = NULL;
    }
    messageName = strdup(name);
    REQ_CONDITION(messageName != NULL, return false);
    return true;
}
void VILLA_ResetMessage() {
    for (int i = 0; i < NUM_MESSAGES; i++) {
        VILLA_FreeMessage(&box[i]);
    }
}
bool VILLA_HaveMessage() {
    for (int i = 0; i < NUM_MESSAGES; i++) {
        if (box[i].string != NULL) return true;
    }
    return false;
}


// INIT & EXIT =========================================================================================================
bool VILLA_InitMessage() {
    font = TTF_OpenFont(MESSAGE_FONT);
    REQ_CONDITION(font != NULL, return false);
    fontHeight = (float)TTF_GetFontHeight(font);

    sprite = IMG_LoadTexture(renderer, "../res/image/test_sprite.png");
    REQ_CONDITION(sprite != NULL, return false);
    SDL_SetTextureScaleMode(sprite, SDL_SCALEMODE_NEAREST);

    return true;
}
void VILLA_ExitMessage() {
    TTF_CloseFont(font); font = NULL;

    SDL_DestroyTexture(sprite); sprite = NULL;
}


// RENEW ===============================================================================================================
static bool VILLA_RenewMessage_Back() {
    back.x = backSpaceOut;
    back.y = windowRect.h - fontHeight * (float)NUM_MESSAGES - backSpaceOut - backSpaceIn * 2;
    back.w = windowRect.w - 2 * backSpaceOut;
    back.h = fontHeight * (float)NUM_MESSAGES + backSpaceIn * 2;
    return true;
}
static bool VILLA_RenewMessage_Box() {
    for (int i = 0; i < NUM_MESSAGES; i++) {
        if (box[i].string == NULL) continue;
        const int len = SDL_min((BASIC_T2 - box[i].time) * CHAR_PER_SEC, strlen(box[i].string));
        if (len <= 0) continue;
        char text[len+1];
        strncpy(text, box[i].string, len);
        text[len] = 0;
        box[i].texture = TXT_LoadTexture(renderer, font, text, WHITE);
        REQ_CONDITION(box[i].texture != NULL, return false);
        box[i].dstRect = (SDL_FRect){
            back.x + backSpaceIn,
            back.y + backSpaceIn + fontHeight * (float)i,
            (float)box[i].texture->w,
            (float)box[i].texture->h
        };
    }
    return true;
}
bool VILLA_RenewMessage() {
    VILLA_RenewMessage_Back();
    VILLA_RenewMessage_Box();
    return true;
}


// DRAW ================================================================================================================
static bool VILLA_DrawMessage_Back() {
    const SDL_FRect edge = {
        back.x - EDGE_WIDTH,
        back.y - EDGE_WIDTH,
        back.w + 2 * EDGE_WIDTH,
        back.h + 2 * EDGE_WIDTH
    };
    SDL_SetRenderColor(renderer, EDGE_COLOR);
    SDL_RenderFillRect(renderer, &edge);

    SDL_SetRenderColor(renderer, BACK_COLOR);
    SDL_RenderFillRect(renderer, &back);

    return true;
}
static bool VILLA_DrawMessage_Box() {
    for (int i = 0; i < NUM_MESSAGES; i++) {
        if (box[i].texture == NULL) continue;

        SDL_SetTextureColorRGB(box[i].texture, TEXT_COLOR);
        SDL_RenderTexture(renderer, box[i].texture, NULL, &box[i].dstRect);

        SDL_DestroyTexture(box[i].texture);
        box[i].texture = NULL;
    }
    return true;
}
static bool VILLA_DrawMessage_Sprite() {
    if (sprite == NULL) return false;
    const float dx = 50, dy = 50;
    const float ATV = BASIC_AtvSin2((BASIC_T2 - startTime) * SPRITE_SPEED);

    const SDL_FRect srcRect = {0, 0, (float)sprite->w, (float)sprite->h};
    SDL_SetTextureAlphaMod(sprite, (int)(ATV * 255));

    SDL_FRect dstRect = srcRect;
    dstRect.w *= SPRITE_SCALE;
    dstRect.h *= SPRITE_SCALE;
    dstRect.x = back.x;
    dstRect.y = back.y - dstRect.h;

    SDL_FRect shadowRect = dstRect;
    shadowRect.y += dy * ATV;
    SDL_SetTextureColorRGB(sprite, BLACK);
    SDL_RenderTexture(renderer, sprite, &srcRect, &shadowRect);

    SDL_FRect spriteRect = dstRect;
    spriteRect.x += dx * ATV;
    SDL_SetTextureColorRGB(sprite, WHITE);
    SDL_RenderTexture(renderer, sprite, &srcRect, &spriteRect);

    return true;
}
bool VILLA_DrawMessage() {
    if (VILLA_HaveMessage() == false) return true;
    VILLA_DrawMessage_Sprite();
    VILLA_DrawMessage_Back();
    VILLA_DrawMessage_Box();
    return true;
}
