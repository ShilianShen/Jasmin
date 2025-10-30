#include "message.h"


#define MESSAGE_FONT "../res/font/IBMPlexMono-Medium.ttf", 48
static TTF_Font* font = NULL;
static SDL_Texture* spriteTex = NULL;


static float startTime = 0.f;


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
static bool VILLA_SetName(const char* name) {
    if (nameTex != NULL) {
        SDL_DestroyTexture(nameTex);
        nameTex = NULL;
    }
    if (name != NULL) {
        nameTex = TXT_LoadTexture(renderer, font, name, WHITE);
        REQ_CONDITION(nameTex != NULL, return false);
    }
    return true;
}
static bool VILLA_SetMail(const int i, const char* string) {
    if (mailMessage[i].string != NULL) {
        free(mailMessage[i].string);
        mailMessage[i].string = NULL;
    }
    if (string != NULL) {
        mailMessage[i].string = strdup(string);
        REQ_CONDITION(mailMessage[i].string != NULL, return false);
        mailMessage[i].time = BASIC_T2;
    }
    return true;
}
bool VILLA_SendMessage(const char* name, const char* string) {
    REQ_CONDITION(string != NULL, return false);

    VILLA_SetName(name);

    for (int i = 0; i < NUM_MESSAGES; i++) {
        if (mailMessage[i].string != NULL) continue;
        VILLA_SetMail(i, string);
        return true;
    }

    for (int i = 0; i < NUM_MESSAGES; i++) {
        VILLA_SetMail(i, NULL);
    }
    VILLA_SetMail(0, string);

    return true;
}
bool VILLA_ClearMessage() {
    bool result = true;
    for (int i = 0; i < NUM_MESSAGES; i++) {
        result = result && VILLA_SetMail(i, NULL);
    }
    return result;
}
bool VILLA_HaveMessage() {
    for (int i = 0; i < NUM_MESSAGES; i++) {
        if (mailMessage[i].string != NULL) return true;
    }
    return false;
}
bool VILLA_SetMessageSprite(SDL_Texture* texture) {
    REQ_CONDITION(texture != NULL, return false);
    spriteTex = texture;
    return true;
}


// INIT & EXIT =========================================================================================================
bool VILLA_InitMessage() {
    font = TTF_OpenFont(MESSAGE_FONT);
    REQ_CONDITION(font != NULL, return false);

    return true;
}
void VILLA_ExitMessage() {
    TTF_CloseFont(font); font = NULL;

    SDL_DestroyTexture(spriteTex); spriteTex = NULL;
}


// RENEW ===============================================================================================================
static bool VILLA_RenewMessage_Mail() {
    static const SDL_FPoint PPP = {100, 100};
    mailRect.w = windowRect.w - 2 * PPP.x;
    mailRect.h = (float)(NUM_MESSAGES * TTF_GetFontHeight(font));
    mailRect.x = PPP.x;
    mailRect.y = windowRect.h - mailRect.h - PPP.y;

    return true;
}
static bool VILLA_RenewMessage_Message() {
    static const int CHAR_PER_SEC = 50;
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
            mailRect.y + (float)(TTF_GetFontHeight(font) * i),
            (float)mailMessage[i].texture->w,
            (float)mailMessage[i].texture->h
        };
    }
    return true;
}
static bool VILLA_RenewMessage_Name() {
    static const SDL_FPoint OFFSET = {-20, 0};
    if (nameTex == NULL) return true;
    nameRect = (SDL_FRect){
        mailRect.x + OFFSET.x,
        mailRect.y + OFFSET.y - (float)TTF_GetFontHeight(font),
        (float)nameTex->w,
        (float)nameTex->h
    };
    return true;
}
bool VILLA_RenewMessage() {
    if (VILLA_HaveMessage() == false) {
        startTime = BASIC_T2;
    }
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
static bool VILLA_DrawMessage_MailName() {
    static const SDL_Color TEXT_COLOR = {100, 100, 100, 255};
    SDL_FRect rects[] = {nameRect, mailRect};
    DrawBox(len_of(rects), rects);

    for (int i = 0; i < NUM_MESSAGES; i++) {
        if (mailMessage[i].texture == NULL) continue;

        SDL_SetTextureColorRGB(mailMessage[i].texture, TEXT_COLOR);
        SDL_RenderTexture(renderer, mailMessage[i].texture, NULL, &mailMessage[i].rect);

        SDL_DestroyTexture(mailMessage[i].texture);
        mailMessage[i].texture = NULL;
    }
    SDL_SetTextureColorRGB(nameTex, TEXT_COLOR);
    SDL_RenderTexture(renderer, nameTex, NULL, &nameRect);
    return true;
}
static bool VILLA_DrawMessage_Sprite() {
    static const float SPRITE_SCALE = 8;
    static const float SPRITE_SPEED = 4;
    static const SDL_FPoint SPRITE_OFFSET = {50, 50};

    if (spriteTex == NULL) return false;
    const float ATV = BASIC_AtvRank2((BASIC_T2 - startTime) * SPRITE_SPEED);
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
    VILLA_DrawMessage_MailName();
    return true;
}
