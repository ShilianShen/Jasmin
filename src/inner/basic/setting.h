#ifndef JASMIN_BASIC_SETTING_H
#define JASMIN_BASIC_SETTING_H



#include "interface.h"


typedef enum JSM_DATA_TYPE {
    JSM_INT, JSM_FLOAT, JSM_STRING, JSM_BOOL, JSM_RECT, JSM_COLOR, JSM_NUM_TYPES
} JSM_DATA_TYPE;
extern SDL_Window* window;
extern SDL_Renderer* renderer;


extern const char* WINDOW_TITLE;
extern const int WINDOW_WIDTH, WINDOW_HEIGHT;
extern bool running;
extern const SDL_WindowFlags FLAG;
SDL_Event event;


extern int logical_w, logical_h;
extern int windowWidth, windowHeight;
extern float scale_x, scale_y;
extern SDL_Color EMPTY;


bool BASIC_Init();
void BASIC_Exit();


#endif //JASMIN_BASIC_SETTING_H