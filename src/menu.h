#ifndef MENU_H
#define MENU_H




// 基本函数
SDL_Texture* getTextureFromImage(SDL_Renderer* renderer, const char* path) {
    // 检查参数
    if (renderer == NULL || path == NULL) {
        printf("Error(getTextureFromImage): Invalid parameters.");
        return NULL;
    }

    // 创建 surface
    SDL_Surface* surface = IMG_Load(path);
    if (surface == NULL) {
        printf("Fail to create surface from %s.", path);
        return NULL;
    }

    // 创建 texture
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (texture == NULL) {
        printf("Fail to create texture from %s.", path);
        return NULL;
    }

    return texture;
}
SDL_Texture* getTextureFromText(SDL_Renderer* renderer, TTF_Font* font, const char* text, const SDL_Color color) {
    // 检查参数
    if (renderer == NULL || font == NULL || text == NULL) {
        printf("Error(getTextureFromText): Invalid parameters.");
        return NULL;
    }

    // 创建 surface
    SDL_Surface* surface = TTF_RenderUTF8_Blended(font, text, color);
    if (surface == NULL) {
        printf("Fail to create surface from %s.", text);
        return NULL;
    }

    // 创建 texture
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (texture == NULL) {
        printf("Fail to create texture from %s.", text);
        return NULL;
    }

    return texture;
}




SDL_Rect bck_rect;




// menuTheme
struct {
    TTF_Font* font;
    SDL_Color color;
} menuTheme;


void loadMenuTheme() {
    // font
    menuTheme.font = TTF_OpenFont("../fonts/Courier New.ttf", 40);
    if (menuTheme.font == NULL) {printf("Fail to load font.");}

    // color
    menuTheme.color.r = 255;
    menuTheme.color.g = 255;
    menuTheme.color.b = 255;
    menuTheme.color.a = 255;
}
void killMenuTheme() {
    if (menuTheme.font != NULL) {TTF_CloseFont(menuTheme.font);}
}




// Elem
typedef uint8_t Anchor;
typedef uint8_t ElemPara;
typedef enum {OUTSIDE, INSIDE, PRESSED, RELEASE, NUM_ELEM_STATES} ElemState;
typedef enum {PASS, FORWARD, BACKWARD, NUM_ELEM_FUNCS} ElemFunc;
const uint8_t ELEM_PATH_LENGTH = 32;


typedef struct {
    // 渲染位置相关
    Anchor anchor;
    SDL_Rect guide;
    SDL_Rect dst_rect;

    // 渲染纹理相关
    SDL_Texture* texture;
    SDL_Rect src_rects[NUM_ELEM_STATES];

    // 功能相关
    ElemState state;
    ElemFunc func;
    ElemPara para;
} Elem;

// load
void loadElemTexture(SDL_Renderer* renderer, Elem* elem, const char* elemString) {
    // 检查参数
    if (renderer == NULL || elem == NULL || elemString == NULL) {return;}
    if (elemString[0] == 0 || elemString[1] == 0 || elemString[2] == 0) {return;}

    // 创建 texture
    switch (elemString[0]) {
        case 't': elem->texture = getTextureFromText(renderer, menuTheme.font, elemString+2, menuTheme.color); break;
        case 'f': elem->texture = getTextureFromImage(renderer, elemString+2); break;
        default: break;
    }
    if (elem->texture == NULL) {return;}

    // 载入 guide, src_rects
    int w, h;
    SDL_QueryTexture(elem->texture, NULL, NULL, &w, &h);
    w /= NUM_ELEM_STATES;
    elem->guide.w = w;
    elem->guide.h = h;
    for (uint8_t i = 0; i < NUM_ELEM_STATES; i++) {
        elem->src_rects[i].x = w * i;
        elem->src_rects[i].y = 0;
        elem->src_rects[i].w = w;
        elem->src_rects[i].h = h;
    }
}

// renew
void renewElemDstRect(Elem* elem) {
    // 检查参数
    if (elem == NULL) return;

    const Anchor x = elem->anchor % 9;
    const Anchor y = elem->anchor / 9;
    elem->dst_rect.w = elem->guide.w;
    elem->dst_rect.h = elem->guide.h;
    Sint16 cx = 0, cy = 0, dx = 0, dy = 0;
    switch (x / 3) {
        case 0: cx = bck_rect.x                  ; break;
        case 1: cx = bck_rect.x + bck_rect.w / 2; break;
        case 2: cx = bck_rect.x + bck_rect.w    ; break;
        default: break;
    }
    switch (y / 3) {
        case 0: cy = bck_rect.y                  ; break;
        case 1: cy = bck_rect.y + bck_rect.h / 2; break;
        case 2: cy = bck_rect.y + bck_rect.h    ; break;
        default: break;
    }
    switch (x % 3) {
        case 0: dx = -elem->dst_rect.w    ; break;
        case 1: dx = -elem->dst_rect.w / 2; break;
        case 2: dx =  0               ; break;
        default: break;
    }
    switch (y % 3) {
        case 0: dy = -elem->dst_rect.h    ; break;
        case 1: dy = -elem->dst_rect.h / 2; break;
        case 2: dy =  0               ; break;
        default: break;
    }
    elem->dst_rect.x = cx + dx;
    elem->dst_rect.y = cy + dy;
}
void renewElemState(Elem* elem) {
    // 检查参数
    if (elem == NULL) return;

    elem->state = mouseInRect(&elem->dst_rect) ? INSIDE : OUTSIDE;
}
void renewElem(Elem* elem) {
    renewElemDstRect(elem);
    renewElemState(elem);
}

// draw
void drawElem(SDL_Renderer* renderer, const Elem* elem) {
    DEBUG_DrawRect(renderer, &elem->dst_rect);
    SDL_RenderCopy(renderer, elem->texture, &elem->src_rects[elem->state], &elem->dst_rect);
};




// Page
struct Page {};


typedef uint8_t PageId;
const uint8_t ElemVol = 255;


void drawPage(SDL_Window* window);




// Menu
const uint8_t MENU_DEPTH = 64;
PageId menu[MENU_DEPTH] = {1, 0};


void drawMenu(SDL_Window* window);




#endif //MENU_H
