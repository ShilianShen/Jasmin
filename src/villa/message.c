#include "message.h"


#define MESSAGE_FONT "../res/font/IBMPlexMono-Medium.ttf", 48
static TTF_Font* font = NULL;
static float fontHeight = 0.f;
static float AAA = 100, BBB = 100;
static SDL_Texture* spriteTex = NULL;
static const float SPRITE_SCALE = 8;


static float startTime = 0.f;
static const SDL_Color TEXT_COLOR = {100, 100, 100, 255};
static const int CHAR_PER_SEC = 50;


static char* messageName = NULL;
static SDL_Texture* nameTex = NULL;
static SDL_FRect nameRect = {0};


typedef struct {
    char* string;
    SDL_Texture* texture;
    SDL_FRect rect;
    float time;
} Message;
static const int NUM_MESSAGES = 4;
static Message mailMessage[NUM_MESSAGES] = {0};
static SDL_FRect mailRect = {0};


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

    if (messageName != NULL) {
        free(messageName);
        messageName = NULL;
    }
    messageName = strdup(name);
    REQ_CONDITION(messageName != NULL, return false);

    for (int i = 0; i < NUM_MESSAGES; i++) {
        if (mailMessage[i].string != NULL) continue;
        mailMessage[i].string = strdup(string);
        REQ_CONDITION(mailMessage[i].string != NULL, return false);
        mailMessage[i].time = BASIC_T2;
        if (i == 0) {
            startTime = BASIC_T2;
        }
        return true;
    }

    VILLA_FreeMessage(&mailMessage[0]);
    for (int i = 1; i < NUM_MESSAGES; i++) {
        mailMessage[i - 1] = mailMessage[i];
    }
    mailMessage[NUM_MESSAGES - 1].string = strdup(string);
    REQ_CONDITION(mailMessage[NUM_MESSAGES - 1].string != NULL, return false);
    mailMessage[NUM_MESSAGES - 1].time = BASIC_T2;

    return true;
}
void VILLA_ResetMessage() {
    for (int i = 0; i < NUM_MESSAGES; i++) {
        VILLA_FreeMessage(&mailMessage[i]);
    }
}
bool VILLA_HaveMessage() {
    for (int i = 0; i < NUM_MESSAGES; i++) {
        if (mailMessage[i].string != NULL) return true;
    }
    return false;
}


// INIT & EXIT =========================================================================================================
bool VILLA_InitMessage() {
    font = TTF_OpenFont(MESSAGE_FONT);
    REQ_CONDITION(font != NULL, return false);
    fontHeight = (float)TTF_GetFontHeight(font);

    spriteTex = IMG_LoadTexture(renderer, "../res/image/test_sprite.png");
    REQ_CONDITION(spriteTex != NULL, return false);
    SDL_SetTextureScaleMode(spriteTex, SDL_SCALEMODE_NEAREST);

    return true;
}
void VILLA_ExitMessage() {
    TTF_CloseFont(font); font = NULL;

    SDL_DestroyTexture(spriteTex); spriteTex = NULL;
}


// RENEW ===============================================================================================================
static bool VILLA_RenewMessage_Mail() {
    mailRect.x = AAA;
    mailRect.y = windowRect.h - fontHeight * (float)NUM_MESSAGES - BBB;
    mailRect.w = windowRect.w - 2 * AAA;
    mailRect.h = fontHeight * (float)NUM_MESSAGES;
    return true;
}
static bool VILLA_RenewMessage_Message() {
    for (int i = 0; i < NUM_MESSAGES; i++) {
        if (mailMessage[i].string == NULL) continue;
        const int len = SDL_min((BASIC_T2 - mailMessage[i].time) * CHAR_PER_SEC, strlen(mailMessage[i].string));
        if (len <= 0) continue;
        char text[len+1];
        strncpy(text, mailMessage[i].string, len);
        text[len] = 0;
        mailMessage[i].texture = TXT_LoadTexture(renderer, font, text, WHITE);
        REQ_CONDITION(mailMessage[i].texture != NULL, return false);
        mailMessage[i].rect = (SDL_FRect){
            mailRect.x,
            mailRect.y + fontHeight * (float)i,
            (float)mailMessage[i].texture->w,
            (float)mailMessage[i].texture->h
        };
    }
    return true;
}
static bool VILLA_RenewMessage_Name() {
    if (messageName == NULL) return true;

    if (nameTex != NULL) {
        SDL_DestroyTexture(nameTex);
        nameTex = NULL;
    }
    nameTex = TXT_LoadTexture(renderer, font, messageName, WHITE);
    REQ_CONDITION(nameTex != NULL, return false);
    nameRect = (SDL_FRect){
        mailRect.x,
        mailRect.y - fontHeight,
        (float)nameTex->w,
        (float)nameTex->h
    };

    return true;
}
bool VILLA_RenewMessage() {
    VILLA_RenewMessage_Mail();
    VILLA_RenewMessage_Name();
    VILLA_RenewMessage_Message();
    return true;
}


// DRAW ================================================================================================================
bool DrawBox(const int N, SDL_FRect rects[N]) {
    static const float BACK_WIDTH = 8;
    static const float EDGE_WIDTH = 8;
    static const SDL_Color EDGE_COLOR = {64, 64, 64, 255};
    static const SDL_Color BACK_COLOR = {225, 225, 225, 255};

    SDL_FRect backs[N], edges[N];
    for (int i = 0; i < N; i++) {
        backs[i] = (SDL_FRect){
            rects[i].x - BACK_WIDTH,
            rects[i].y - BACK_WIDTH,
            rects[i].w + BACK_WIDTH * 2,
            rects[i].h + BACK_WIDTH * 2
        };
        edges[i] = (SDL_FRect){
            backs[i].x - EDGE_WIDTH,
            backs[i].y - EDGE_WIDTH,
            backs[i].w + EDGE_WIDTH * 2,
            backs[i].h + EDGE_WIDTH * 2
        };
    }

    return true
    && SDL_SetRenderColor(renderer, EDGE_COLOR)
    && SDL_RenderFillRects(renderer, edges, N)
    && SDL_SetRenderColor(renderer, BACK_COLOR)
    && SDL_RenderFillRects(renderer, backs, N)
    ;
}


static bool VILLA_DrawMessage_Mail() {
    SDL_FRect rects[] = {nameRect, mailRect};
    DrawBox(len_of(rects), rects);

    for (int i = 0; i < NUM_MESSAGES; i++) {
        if (mailMessage[i].texture == NULL) continue;

        SDL_SetTextureColorRGB(mailMessage[i].texture, TEXT_COLOR);
        SDL_RenderTexture(renderer, mailMessage[i].texture, NULL, &mailMessage[i].rect);

        SDL_DestroyTexture(mailMessage[i].texture);
        mailMessage[i].texture = NULL;
    }
    return true;
}
static bool VILLA_DrawMessage_Name() {
    SDL_SetTextureColorRGB(nameTex, TEXT_COLOR);
    SDL_RenderTexture(renderer, nameTex, NULL, &nameRect);
    return true;
}
static bool VILLA_DrawMessage_Sprite() {
    static const float SPRITE_SPEED = 4;
    static const SDL_FPoint SPRITE_OFFSET = {50, 50};

    if (spriteTex == NULL) return false;
    const float ATV = BASIC_AtvSin2((BASIC_T2 - startTime) * SPRITE_SPEED);
    SDL_SetTextureAlphaMod(spriteTex, (int)(ATV * 255));
    const SDL_FRect srcRect = {0, 0, (float)spriteTex->w, (float)spriteTex->h};

    SDL_FRect dstRect = srcRect;
    dstRect.w *= SPRITE_SCALE;
    dstRect.h *= SPRITE_SCALE;
    dstRect.x = mailRect.x;
    dstRect.y = mailRect.y - dstRect.h;

    SDL_FRect shadowRect = dstRect;
    shadowRect.y += SPRITE_OFFSET.y * ATV;
    SDL_SetTextureColorRGB(spriteTex, BLACK);
    SDL_RenderTexture(renderer, spriteTex, &srcRect, &shadowRect);

    SDL_FRect spriteRect = dstRect;
    spriteRect.x += SPRITE_OFFSET.x * ATV;
    SDL_SetTextureColorRGB(spriteTex, WHITE);
    SDL_RenderTexture(renderer, spriteTex, &srcRect, &spriteRect);

    return true;
}
bool VILLA_DrawMessage() {
    if (VILLA_HaveMessage() == false) return true;
    VILLA_DrawMessage_Sprite();
    VILLA_DrawMessage_Mail();
    VILLA_DrawMessage_Name();
    return true;
}
