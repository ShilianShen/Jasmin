#include "menu.h"


const char* MENU_ROOT_NAME = "Root";
const char* MENU_EDGE_NAME = "Edge";
static const char* tomlPath = "../config/menu.toml";


// MENU TRIG TYPE ======================================================================================================
enum MenuTrig {
    MENU_TRIG_PASS,
    MENU_TRIG_FORWARD,
    MENU_TRIG_BACKWARD,
    MENU_TRIG_CLEAR,
    MENU_NUM_TRIGS,
};
const char* MENU_TRIG_STRINGS[MENU_NUM_TRIGS] = {
    [MENU_TRIG_PASS] = "pass",
    [MENU_TRIG_FORWARD] = "forward",
    [MENU_TRIG_BACKWARD] = "backward",
    [MENU_TRIG_CLEAR] = "clear",
};


// MENU ================================================================================================================
Menu menu;


// GET & SET ===========================================================================================================



// LOAD & UNLOAD =======================================================================================================
static void TEMPO_LoadMenuPageSet() {
    toml_table_t* tomlPageSet = getToml(tomlPath);
    if (tomlPageSet == NULL) {
        printf("%s: tomlMenu == NULL.\n", __func__);
        return;
    } // Req Condition

    for (int i = 0; toml_key_in(tomlPageSet, i) != NULL; i++) {
        menu.lenPageSet = i;
    }
    if (menu.lenPageSet == 0) {
        printf("%s: menu.lenPageSet == 0.\n", __func__);
        return;
    } // Req Condition

    menu.pageSet = calloc(menu.lenPageSet, sizeof(Page*));
    if (menu.pageSet == NULL) {
        printf("%s: menu.pageSet == NULL.\n", __func__);
        return;
    }

    for (int i = 0; i < menu.lenPageSet; i++) {
        const char* key = toml_key_in(tomlPageSet, i);
        const toml_table_t* tomlPage = toml_table_in(tomlPageSet, key);
        if (tomlPage == NULL) {
            printf("%s: tomlPage == NULL.\n", __func__);
            return;
        } // Req Condition


        menu.pageSet[i] = TEMPO_CreatePage(key, tomlPage);
        if (strcmp(key, MENU_ROOT_NAME) == 0) {
            menu.pageRoot = menu.pageSet[i];
        }
        if (strcmp(key, MENU_EDGE_NAME) == 0) {
            menu.pageEdge = menu.pageSet[i];
        }
    }

    //
    toml_free(tomlPageSet); // end malloc
}
void TEMPO_LoadMenu() {
    memset(&menu, 0, sizeof(Menu));
    TEMPO_LoadMenuPageSet();
}
void TEMPO_UnloadMenu() {
    for (int i = 0; i < menu.lenPageSet; i++) {
        menu.pageSet[i] = TEMPO_DeletePage(menu.pageSet[i]);
    }
}


// RENEW ===============================================================================================================
static void TEMPO_RenewMenuPath() {
    //
    bool need_clear = false;
    DEBUG_SendMessageL("tempo.path: /%s", TEMPO_GetPageName(menu.pageRoot));
    for (int i = 0; i < MENU_PATH_VOLUME; i++) {
        if (need_clear)
            menu.path[i] = NULL;
        else if (menu.path[i] == NULL)
            need_clear = true;
        else
            DEBUG_SendMessageL("/%s", TEMPO_GetPageName(menu.path[i]));
    }
    if (need_clear == false) {
        DEBUG_SendMessageL("/%s", TEMPO_GetPageName(menu.pageEdge));
    }
    DEBUG_SendMessageL("\n");
}
static void TEMPO_RenewMenuPageNow() {
    // if pageRoot
    if (menu.path[0] == NULL) {
        menu.pageNow = menu.pageRoot;
        return;
    }
    // else if pageOther
    for (int i = 0; i + 1 < MENU_PATH_VOLUME; i++) {
        if (menu.path[i] != NULL && menu.path[i+1] == NULL) {
            menu.pageNow = menu.path[i];
            return;
        }
    }
    // else pageEdge
    menu.pageNow = menu.pageEdge;
}
void TEMPO_RenewMenu() {
    TEMPO_RenewMenuPath();
    TEMPO_RenewMenuPageNow();
    TEMPO_RenewPage(menu.pageNow);
}


// DRAW ================================================================================================================
void TEMPO_DrawMenu() {
    TEMPO_DrawPage(menu.pageNow);
}


// TRIG ================================================================================================================
void TEMPO_TrigFuncPass(TrigPara para) {}
void TEMPO_TrigFuncForward(TrigPara para) {
    const char* pageName = para.pageName;
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
void TEMPO_TrigFuncBackward(TrigPara para) {
    for (int i = MENU_PATH_VOLUME - 1; i >= 0; i--) {
        if (menu.path[i] != 0) {
            menu.path[i] = 0;
            break;
        }
    }
}
void TEMPO_TrigFuncClear(TrigPara para) {
    for (int i = 0; i < MENU_PATH_VOLUME; i++) {menu.path[i] = 0;}
}
void TEMPO_TrigFuncKnob(TrigPara para) {

}