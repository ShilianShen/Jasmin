#include "../menu/menu.h"


Trig2 trig_set[] = {
    {"pass", TRIG_FUNC_Pass},
    {"forward", TRIG_FUNC_Forward},
    {"backward", TRIG_FUNC_Backward},
    {"clear", TRIG_FUNC_Clear},
    {NULL, NULL}
};


void TRIG_FUNC_Pass(const char* para) {}
void TRIG_FUNC_Forward(const char* pageName) {
    // getPageId
    int pageId = 0;
    for (int i = 0; i < menu.lenPageSet; i++) {
        if (menu.pageSet[i] == NULL) {continue;}
        if (strcmp(TEMPO_GetPageName(menu.pageSet[i]), pageName) == 0) {pageId = i;}
    }
    if (pageId == 0) {printf("%s: \"%s\" not exists.\n", __func__, (char*)pageName); return;}

    // forward
    for (int i = 0; i < MENU_PATH_VOLUME; i++) {
        if (menu.path[i] == 0) {
            menu.path[i] = menu.pageSet[pageId];
            break;
        }
    }
}
void TRIG_FUNC_Backward(const char* para) {
    for (int i = MENU_PATH_VOLUME - 1; i >= 0; i--) {
        if (menu.path[i] != 0) {
            menu.path[i] = 0;
            break;
        }
    }
}
void TRIG_FUNC_Clear(const char* para) {
    for (int i = 0; i < MENU_PATH_VOLUME; i++) {menu.path[i] = 0;}
}


TrigFunc TRIG_FindFuncFromName(const char* name) {
    // Req Condition
    if (name == NULL) {printf("%s: name not exists.\n", __func__); return NULL;}

    //
    for (int i = 0; trig_set[i].name != NULL; i++) {
        if (strcmp(trig_set[i].name, name) == 0) {
            return trig_set[i].func;
        }
    }
    return NULL;
}
char* TRIG_FindNameFromFunc(const TrigFunc func) {
    // Req Condition
    if (func == NULL) {printf("%s: func not exists.\n", __func__); return NULL;}

    //
    for (int i = 0; trig_set[i].func != NULL; i++) {
        if (trig_set[i].func == func) {
            return trig_set[i].name;
        }
    }
    return NULL;
}