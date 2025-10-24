#include "mouse.h"


static struct {
    struct {
        SDL_FPoint pos, leftPos, rightPos;
        bool leftPressed, rightPressed;
    } state1, state2;
    Trig left_trig;
    Trig right_trig;
    SDL_Texture* tex;
    SDL_FRect texDstRect;
    cJSON* json;
} mouse;


// SET & GET ===========================================================================================================
SDL_FPoint PERPH_GetMousePos() {
    return mouse.state2.pos;
}
void PERPH_SetMouseLeftTrig(const Trig trig) {
    mouse.left_trig = trig;
}
bool PERPH_GetMouseLeftInRect(const SDL_FRect rect) {
    return mouse.state2.leftPressed && SDL_GetPointInRect(mouse.state2.leftPos, rect);
}
bool PERPH_GetMouseInRect(const SDL_FRect rect) {
    return SDL_GetPointInRect(mouse.state2.pos, rect);
}
bool PERPH_GetMouseLeftPressed() {
    return mouse.state2.leftPressed;
}
bool PERPH_GetMouseLeftPress() {
    return mouse.state1.leftPressed == false && mouse.state2.leftPressed == true;
}


// INIT & EXIR =========================================================================================================
bool PERPH_InitMouse() {
    memset(&mouse, 0, sizeof(mouse));
    mouse.json = getJson(PERPH_MOUSE_JSON);
    REQ_CONDITION(mouse.json != NULL, return false);

    char* tex_json = NULL;
    if (cJSON_Load(mouse.json, "tex", JSM_STRING, &tex_json)) {
        float scale = 0;
        REQ_CONDITION(cJSON_Load(mouse.json, "scale", JSM_FLOAT, &scale), return false);

        mouse.tex = IMG_LoadTexture(renderer, tex_json);
        REQ_CONDITION(mouse.tex != NULL, return false);
        tex_json = NULL;
        SDL_SetTextureScaleMode(mouse.tex, SDL_SCALEMODE_NEAREST);
        SDL_HideCursor();

        mouse.texDstRect.w = scale * (float)mouse.tex->w;
        mouse.texDstRect.h = scale * (float)mouse.tex->h;
    }
    cJSON_Delete(mouse.json);
    mouse.json = NULL;
    return true;
}
void PERPH_ExitMouse() {
    if (mouse.json != NULL) {
        cJSON_Delete(mouse.json);
        mouse.json = NULL;
    }
    if (mouse.tex != NULL) {
        SDL_DestroyTexture(mouse.tex);
        mouse.tex = NULL;
    }
}


// RENEW ===============================================================================================================
static void PERPH_RenewMouse_State() {
    mouse.state1 = mouse.state2;

    float x, y;
    const SDL_MouseButtonFlags buttons = SDL_GetMouseState(&x, &y);
    mouse.state2.pos = (SDL_FPoint){x * windowScale.x, y * windowScale.y};
    mouse.state2.leftPressed = buttons & SDL_BUTTON_LMASK;
    mouse.state2.rightPressed = buttons & SDL_BUTTON_RMASK;

    if (!mouse.state1.leftPressed && mouse.state2.leftPressed) mouse.state2.leftPos = mouse.state2.pos;
}
static void PERPH_RenewMouse_Trig() {
    if (mouse.left_trig.func == NULL) return;

    if (mouse.left_trig.sustain == false
        && mouse.state1.leftPressed == true
        && mouse.state2.leftPressed == false
        ) {
        ma_engine_play_sound(&engine, "../res/sound/switch.wav", NULL);
        BASIC_PullTrig(mouse.left_trig);
        }

    if (mouse.left_trig.sustain == true
        && mouse.state1.leftPressed == true
        ) {
        if (mouse.state2.leftPressed == false) ma_engine_play_sound(&engine, "../res/sound/switch.wav", NULL);
        BASIC_PullTrig(mouse.left_trig);
        }
    mouse.left_trig = (Trig){0};
}
bool PERPH_RenewMouse() {
    PERPH_RenewMouse_State();
    PERPH_RenewMouse_Trig();
    return true;
}


// DRAW ================================================================================================================
bool PERPH_DrawMouse() {
    DEBUG_SendMessageL("%s:\n", __func__);
    DEBUG_SendMessageL("    mousePos: %s\n", SDL_GetStrFPoint(mouse.state2.pos));
    if (mouse.left_trig.func != NULL) DEBUG_SendMessageL("    mouse.left_trig: READY.\n");

    DEBUG_DrawPoint(mouse.state2.pos);
    if (mouse.state2.leftPressed) {
        DEBUG_SendMessageL("    mouseLeftPos: %s\n", SDL_GetStrFPoint(mouse.state2.leftPos));
        DEBUG_DrawPoint(mouse.state2.leftPos);
        DEBUG_DrawLine(mouse.state2.leftPos, mouse.state2.pos);
    }
    mouse.texDstRect.x = mouse.state2.pos.x;
    mouse.texDstRect.y = mouse.state2.pos.y;
    SDL_RenderTexture(renderer, mouse.tex, NULL, &mouse.texDstRect);
    return true;
}
