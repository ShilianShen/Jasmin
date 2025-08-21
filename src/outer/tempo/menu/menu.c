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
static bool TEMPO_LoadMenuPageSet(const toml_table_t* tomlPageSet) {

    if (tomlPageSet == NULL) {
        printf("%s: tomlMenu == NULL.\n", __func__);
        return false;
    } // Req Condition

    for (int i = 0; toml_key_in(tomlPageSet, i) != NULL; i++) {
        menu.lenPageTable = i + 1;
    }
    if (menu.lenPageTable == 0) {
        printf("%s: menu.lenPageSet == 0.\n", __func__);
        return false;
    } // Req Condition

    menu.pageTable = calloc(menu.lenPageTable, sizeof(KeyVal));
    if (menu.pageTable == NULL) {
        printf("%s: menu.pageSet == NULL.\n", __func__);
        return false;
    }

    for (int i = 0; i < menu.lenPageTable; i++) {
        const char* key = toml_key_in(tomlPageSet, i);
        menu.pageTable[i].key = strdup(key);
        if (menu.pageTable[i].key == NULL) {
            return false;
        }
    }

    for (int i = 0; i < menu.lenPageTable; i++) {
        const char* key = toml_key_in(tomlPageSet, i);
        const toml_table_t* tomlPage = toml_table_in(tomlPageSet, key);
        if (tomlPage == NULL) {
            printf("%s: tomlPage == NULL.\n", __func__);
            return false;
        } // Req Condition

        menu.pageTable[i].val = TEMPO_CreatePage(key, tomlPage);
        if (strcmp(key, MENU_ROOT_NAME) == 0) {
            menu.pageRoot = menu.pageTable[i].val;
        }
        if (strcmp(key, MENU_EDGE_NAME) == 0) {
            menu.pageEdge = menu.pageTable[i].val;
        }
    }


    return true;
}
void TEMPO_LoadMenu() {
    memset(&menu, 0, sizeof(Menu));
    toml_table_t* tomlPageSet = getToml(tomlPath);
    TEMPO_LoadMenuPageSet(tomlPageSet);
    toml_free(tomlPageSet); // end malloc
}
void TEMPO_UnloadMenu() {
    for (int i = 0; i < menu.lenPageTable; i++) {
        free(menu.pageTable[i].key);
        menu.pageTable[i].key = NULL;
        menu.pageTable[i].val = TEMPO_DeletePage(menu.pageTable[i].val);
    }
}


// RENEW ===============================================================================================================
static void TEMPO_RenewMenuPath() {
    //
    bool need_clear = false;
    DEBUG_SendMessageL("tempo.path: /%s", BASIC_GetKeyByVal(menu.pageTable, menu.pageRoot));
    for (int i = 0; i < MENU_PATH_VOLUME; i++) {
        if (need_clear)
            menu.path[i] = NULL;
        else if (menu.path[i] == NULL)
            need_clear = true;
        else
            DEBUG_SendMessageL("/%s", BASIC_GetKeyByVal(menu.pageTable, menu.path[i]));
    }
    if (need_clear == false) {
        DEBUG_SendMessageL("/%s", BASIC_GetKeyByVal(menu.pageTable, menu.pageEdge));
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
void TEMPO_TrigFuncPass(const char* para) {}
void TEMPO_TrigFuncForward(const char* para) {
    if (para == NULL) {
        printf("%s: para == NULL.\n", __func__);
        return;
    }
    const char* pageName = para;
    // getPageId
    int pageId = 0;
    for (int i = 0; i < menu.lenPageTable; i++) {
        if (menu.pageTable[i].val == NULL) {continue;}
        if (strcmp(menu.pageTable[i].key, pageName) == 0) {pageId = i;}
    }
    if (pageId == 0) {printf("%s: \"%s\" not exists.\n", __func__, (char*)pageName); return;}

    // forward
    for (int i = 0; i < MENU_PATH_VOLUME; i++) {
        if (menu.path[i] == 0) {
            menu.path[i] = menu.pageTable[pageId].val;
            break;
        }
    }
}
void TEMPO_TrigFuncBackward(const char* para) {
    for (int i = MENU_PATH_VOLUME - 1; i >= 0; i--) {
        if (menu.path[i] != 0) {
            menu.path[i] = 0;
            break;
        }
    }
}
void TEMPO_TrigFuncClear(const char* para) {
    for (int i = 0; i < MENU_PATH_VOLUME; i++) {menu.path[i] = 0;}
}