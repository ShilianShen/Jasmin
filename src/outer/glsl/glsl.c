#include "glsl.h"
#include <SDL3/SDL_opengl.h>



bool GLSL_Init() {
    return true;
}
bool GLSL_Renew() {
    return true;
}
bool GLSL_Draw() {
    glClearColor(1, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    if (SDL_GL_SwapWindow(window) == false) {
        SDL_Log("%s: %s\n", __func__, SDL_GetError());
        return false;
    }
    return true;
}
void GLSL_Exit() {

}