#ifndef JASMIN__BASIC_H
#define JASMIN__BASIC_H

#define len_of(x) (sizeof(x) / sizeof((x)[0]))
#define REQ_CONDITION(expr, stmt)                          \
    do {                                                   \
        if (!(expr)) {                                     \
            printf("%s: %s == false.\n", __func__, #expr); \
            stmt;                                          \
        }                                                  \
    } while (0)

#include "../third_party/third_party.h"


typedef enum {
    JSM_VOID,
    JSM_PTR,
    JSM_INT,
    JSM_FLOAT,
    JSM_STRING,
    JSM_BOOL,
    JSM_FRECT,
    JSM_RECT,
    JSM_COLOR,
    JSM_NUM_TYPES
} JSM_DataType;


extern SDL_FRect windowRectScaled, windowRect;
extern ma_engine engine;


void MA_PlaySound(const char *sound);


extern SDL_Window* window;
extern SDL_Renderer* renderer;


extern const char* WINDOW_TITLE;
extern const int WINDOW_WIDTH, WINDOW_HEIGHT;
extern bool running;
extern const SDL_WindowFlags FLAG;
SDL_Event sdl_event;
extern SDL_GLContext content;


extern int logical_w, logical_h;
extern int windowWidth, windowHeight;
extern float scale_x, scale_y;
extern SDL_Color EMPTY;


#endif //JASMIN__BASIC_H