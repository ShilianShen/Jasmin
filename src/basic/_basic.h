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
#define OPT_CONDITION(expr, stmt)                          \
    do {                                                   \
        if (!(expr)) {                                     \
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


void MA_PlaySound(const char *sound);


extern const char* WINDOW_TITLE;
extern const int WINDOW_WIDTH;
extern const int WINDOW_HEIGHT;
extern SDL_Window* window;
extern SDL_FRect windowRect;
extern ma_engine engine;
extern SDL_Renderer* renderer;
extern bool running;
extern SDL_Event sdl_event;
extern float BASIC_T1, BASIC_T2, BASIC_DT;
extern SDL_FPoint windowScale;
extern SDL_Color EMPTY, BLACK, WHITE, RED, GREEN, BLUE;


#endif //JASMIN__BASIC_H