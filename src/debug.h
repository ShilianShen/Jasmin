#ifndef DEBUG_H
#define DEBUG_H


// debug parameters
struct {
    bool on;
    SDL_Color colorPoint;
    SDL_Color colorLine;
    SDL_Color colorFace;
    SDL_Color colorText;
} debug;
void loadDebug() {
    debug.on = true;
    debug.colorPoint = (SDL_Color){246, 202, 124, 255};
    debug.colorLine = (SDL_Color){158, 189, 127, 255};
    debug.colorFace = (SDL_Color){241, 155, 153, 255};
    debug.colorText = (SDL_Color){116, 173, 220, 255};
};


// debug functions
void DEBUG_DrawPoint(SDL_Renderer* renderer, const Sint16 x, const Sint16 y) {
    if (!debug.on || renderer == NULL) {return;}

    const float w = 4;
    const SDL_FRect rect = {x - w, y - w, 2 * w, 2 * w};
    SDL_SetRenderDrawColor(renderer, debug.colorPoint.r, debug.colorPoint.g, debug.colorPoint.b, debug.colorPoint.a);
    SDL_RenderFillRect(renderer, &rect);
}
void DEBUG_DrawRect(SDL_Renderer* renderer, const SDL_FRect* rect) {
    if (!debug.on || renderer == NULL || rect == NULL) {return;}

    SDL_SetRenderDrawColor(renderer, debug.colorFace.r, debug.colorFace.g, debug.colorFace.b, debug.colorFace.a);
    SDL_RenderRect(renderer, rect);
}


#endif //DEBUG_H
