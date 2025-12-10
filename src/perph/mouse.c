#include "mouse.h"



static const char* MOUSE_KEY_NAMES[PERPH_NUM_MOUSE_KEYS] = {
    [PERPH_MOUSE_KEY_LEFT] = "Left",
    [PERPH_MOUSE_KEY_RIGHT] = "Right",
};
static struct {
    struct {
        SDL_FPoint posNow;
        SDL_FPoint pos[PERPH_NUM_MOUSE_KEYS];
        bool pressed[PERPH_NUM_MOUSE_KEYS];
    } state1, state2;
    Trig trig[PERPH_NUM_MOUSE_KEYS];
    SDL_Texture* texture;
    SDL_FRect dstRect;
} mouse;


// GET & SET ===========================================================================================================
SDL_FPoint PERPH_GetMousePos() {
    return mouse.state2.posNow;
}
SDL_FPoint PERPH_GetMouseKeyPos(const PERPH_MouseKey key) {
    return mouse.state2.pos[key];
}
bool PERPH_SetMouseKeyTrig(const PERPH_MouseKey key, const Trig trig) {
    REQ_CONDITION(0 <= key && key < PERPH_NUM_MOUSE_KEYS, return false);
    if (mouse.trig[key].func != NULL) return false;
    mouse.trig[key] = trig;
    return true;
}
bool PERPH_GetMouseInRect(const SDL_FRect rect) {
    return SDL_GetPointInRect(mouse.state2.posNow, rect);
}
bool PERPH_GetMouseKeyInRect(const PERPH_MouseKey key, const SDL_FRect rect) {
    return true
    && mouse.state2.pressed[key]
    && SDL_GetPointInRect(mouse.state2.pos[key], rect)
    ;
}
bool PERPH_GetMouseAndKeyInRect(const PERPH_MouseKey key, const SDL_FRect rect) {
    return PERPH_GetMouseInRect(rect) && PERPH_GetMouseKeyInRect(key, rect);
}
bool PERPH_GetMouseKeyPressed(const PERPH_MouseKey key) {
    return mouse.state2.pressed[key];
}
bool PERPH_GetMouseKeyPress(const PERPH_MouseKey key) {
    return mouse.state1.pressed[key] == false && mouse.state2.pressed[key] == true;
}


// INIT & EXIT =========================================================================================================
bool PERPH_InitMouse(const cJSON* mouse_json) {
    memset(&mouse, 0, sizeof(mouse));

    REQ_CONDITION(mouse_json != NULL, return false);

    char* tex_json = NULL;
    if (cJSON_LoadByKey(mouse_json, "texture", JSM_STRING, &tex_json)) {
        float scale = 0;
        REQ_CONDITION(cJSON_LoadByKey(mouse_json, "scale", JSM_FLOAT, &scale), return false);

        mouse.texture = IMG_LoadTexture(renderer, tex_json);
        REQ_CONDITION(mouse.texture != NULL, return false);
        tex_json = NULL;
        SDL_SetTextureScaleMode(mouse.texture, SDL_SCALEMODE_NEAREST);
        SDL_HideCursor();

        mouse.dstRect.w = scale * (float)mouse.texture->w;
        mouse.dstRect.h = scale * (float)mouse.texture->h;
    }
    return true;
}
void PERPH_ExitMouse() {
    if (mouse.texture != NULL) {
        SDL_DestroyTexture(mouse.texture);
        mouse.texture = NULL;
    }
}


// RENEW ===============================================================================================================
static void PERPH_RenewMouse_State() {
    mouse.state1 = mouse.state2;

    float x, y;
    const SDL_MouseButtonFlags buttons = SDL_GetMouseState(&x, &y);
    mouse.state2.posNow = (SDL_FPoint){x * windowScale.x, y * windowScale.y};
    mouse.state2.pressed[PERPH_MOUSE_KEY_LEFT] = buttons & SDL_BUTTON_LMASK;
    mouse.state2.pressed[PERPH_MOUSE_KEY_RIGHT] = buttons & SDL_BUTTON_RMASK;

    if (!mouse.state1.pressed[PERPH_MOUSE_KEY_LEFT] && mouse.state2.pressed[PERPH_MOUSE_KEY_LEFT]) mouse.state2.pos[PERPH_MOUSE_KEY_LEFT] = mouse.state2.posNow;
    if (!mouse.state1.pressed[PERPH_MOUSE_KEY_RIGHT] && mouse.state2.pressed[PERPH_MOUSE_KEY_RIGHT]) mouse.state2.pos[PERPH_MOUSE_KEY_RIGHT] = mouse.state2.posNow;
}
static void PERPH_RenewMouse_Trig() {
    for (int key = 0; key < PERPH_NUM_MOUSE_KEYS; key++) {
        if (mouse.trig[key].func == NULL) continue;

        if (mouse.trig[key].sustain == false
            && mouse.state1.pressed[key] == true
            && mouse.state2.pressed[key] == false
            ) {
            ma_engine_play_sound(&engine, "../res/sound/switch.wav", NULL);
            BASIC_PullTrig(mouse.trig[key]);
            }

        if (mouse.trig[key].sustain == true
            && mouse.state1.pressed[key] == true
            ) {
            if (mouse.state2.pressed[key] == false) ma_engine_play_sound(&engine, "../res/sound/switch.wav", NULL);
            BASIC_PullTrig(mouse.trig[key]);
            }
        mouse.trig[key] = (Trig){0};
    }
}
bool PERPH_RenewMouse() {
    PERPH_RenewMouse_State();
    PERPH_RenewMouse_Trig();
    return true;
}


// DRAW ================================================================================================================
bool PERPH_DrawMouse() {
    DEBUG_SendMessageL("%s:\n", __func__);
    DEBUG_SendMessageL("    posNow: %s\n", SDL_GetStrFPoint(mouse.state2.posNow));

    for (int key = 0; key < PERPH_NUM_MOUSE_KEYS; key++) {
        if (mouse.trig[key].func != NULL)
            DEBUG_SendMessageL("    trig[%s]: READY.\n", MOUSE_KEY_NAMES[key]);

        if (mouse.state2.pressed[key]) {
            DEBUG_SendMessageL("    pos[%s]: %s\n", MOUSE_KEY_NAMES[key], SDL_GetStrFPoint(mouse.state2.pos[key]));
            DEBUG_DrawPoint(mouse.state2.pos[key]);
            DEBUG_DrawLine(mouse.state2.pos[key], mouse.state2.posNow);
        }
    }

    DEBUG_DrawPoint(mouse.state2.posNow);

    mouse.dstRect.x = mouse.state2.posNow.x;
    mouse.dstRect.y = mouse.state2.posNow.y;
    SDL_RenderTexture(renderer, mouse.texture, NULL, &mouse.dstRect);
    return true;
}
