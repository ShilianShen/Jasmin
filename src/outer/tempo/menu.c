#include "menu.h"


const char* MENU_ROOT_NAME = "Root";
const char* MENU_EDGE_NAME = "Edge";
#define MENU_PATH_VOLUME 6
struct Menu {
    // malloc in INIT
    int lenPageTable;
    KeyVal* pageTable;
    Page* pageRoot;
    Page* pageEdge;

    // path
    Page* path[MENU_PATH_VOLUME];
    Page* pageNow;
};
typedef struct Menu Menu;


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
static bool TEMPO_LoadMenu_PageSet(const cJSON* pageSet_json) {
    if (pageSet_json == NULL) {
        printf("%s: tomlMenu == NULL.\n", __func__);
        return false;
    } // Req Condition

    menu.lenPageTable = cJSON_GetArraySize(pageSet_json);
    if (menu.lenPageTable == 0) {
        printf("%s: menu.lenPageSet == 0.\n", __func__);
        return false;
    } // Req Condition

    menu.pageTable = calloc(menu.lenPageTable, sizeof(KeyVal));
    if (menu.pageTable == NULL) {
        printf("%s: menu.pageSet == NULL.\n", __func__);
        return false;
    } // Req Condition

    for (int i = 0; i < menu.lenPageTable; i++) {
        const cJSON *item = cJSON_GetArrayItem(pageSet_json, i);
        if (item == NULL) {
            printf("%s: item == NULL.\n", __func__);
            return false;
        } // Req Condition
        menu.pageTable[i].key = strdup(item->string);
        if (menu.pageTable[i].key == NULL) {
            printf("%s: menu.pageTable[i].key == NULL.\n", __func__);
            return false;
        } // Req Condition
    }

    for (int i = 0; i < menu.lenPageTable; i++) {
        const cJSON *item = cJSON_GetArrayItem(pageSet_json, i);
        if (item == NULL) {
            printf("%s: item == NULL.\n", __func__);
            return false;
        } // Req Condition
        const char* key = item->string;
        const cJSON* page_json = cJSON_GetObjectItem(pageSet_json, key);
        if (page_json == NULL) {
            printf("%s: tomlPage == NULL.\n", __func__);
            return false;
        } // Req Condition

        menu.pageTable[i].val = TEMPO_CreatePage(page_json);
        if (strcmp(key, MENU_ROOT_NAME) == 0) {
            menu.pageRoot = menu.pageTable[i].val;
        } // Opt Condition
        if (strcmp(key, MENU_EDGE_NAME) == 0) {
            menu.pageEdge = menu.pageTable[i].val;
        } // Opt Condition
    }


    return true;
}
static bool TEMPO_LoadMenu_RK(const cJSON* menu_json) {
    const cJSON* pageSet_json = cJSON_GetObjectItem(menu_json, "pageTable");
    if (pageSet_json == NULL) {
        printf("%s: cJSON_GetObjectItem == NULL.\n", __func__);
        return false;
    } // Req Condition

    if (TEMPO_LoadMenu_PageSet(pageSet_json) == false) {
        printf("%s: TEMPO_LoadMenu_PageSet == false.\n", __func__);
        return false;
    } // Req Condition

    return true;
}

bool TEMPO_LoadMenu() {
    memset(&menu, 0, sizeof(Menu));
    cJSON* menu_json = getJson(TEMPO_MENU_JSON);
    if (menu_json == NULL) {
        printf("%s: getJson == NULL.\n", __func__);
        return false;
    } // Req Condition

    const bool rk = TEMPO_LoadMenu_RK(menu_json);
    cJSON_Delete(menu_json);

    if (rk == false) {
        TEMPO_UnloadMenu();
        return false;
    }
    return true;
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
    DEBUG_SendMessageL("tempo.path: /%s", TABLE_GetKeyByVal((Table){menu.lenPageTable, menu.pageTable}, menu.pageRoot));
    for (int i = 0; i < MENU_PATH_VOLUME; i++) {
        if (need_clear)
            menu.path[i] = NULL;
        else if (menu.path[i] == NULL)
            need_clear = true;
        else
            DEBUG_SendMessageL("/%s", TABLE_GetKeyByVal((Table){menu.lenPageTable, menu.pageTable}, menu.path[i]));
    }
    if (need_clear == false) {
        DEBUG_SendMessageL("/%s", TABLE_GetKeyByVal((Table){menu.lenPageTable, menu.pageTable}, menu.pageEdge));
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

bool TEMPO_RenewMenu() {
    TEMPO_RenewMenuPath();
    TEMPO_RenewMenuPageNow();

    if (menu.path[0] == NULL) {
        TEMPO_RenewPage(menu.pageRoot);
    }
    else if (menu.path[MENU_PATH_VOLUME-1] != NULL) {
        TEMPO_RenewPage(menu.pageEdge);
    }
    else {
        for (int i = 0; menu.path[i] != NULL; i++) {
            TEMPO_RenewPage(menu.path[i]);
        }
    }
    return true;
}


// DRAW ================================================================================================================
void TEMPO_DrawMenu() {

    if (menu.path[0] == NULL) {
        TEMPO_DrawPage(menu.pageRoot);
    }
    else if (menu.path[MENU_PATH_VOLUME-1] != NULL) {
        TEMPO_DrawPage(menu.pageEdge);
    }
    else {
        for (int i = 0; menu.path[i] != NULL; i++) {
            TEMPO_DrawPage(menu.path[i]);
        }
    }
}


// TRIG ================================================================================================================
static void TEMPO_TrigFuncPass(const char* para) {}
static void TEMPO_TrigFuncForward(const char* para) {
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
static void TEMPO_TrigFuncBackward(const char* para) {
    for (int i = MENU_PATH_VOLUME - 1; i >= 0; i--) {
        if (menu.path[i] != 0) {
            menu.path[i] = 0;
            break;
        }
    }
}
static void TEMPO_TrigFuncClear(const char* para) {
    for (int i = 0; i < MENU_PATH_VOLUME; i++) {menu.path[i] = 0;}
}
static const KeyVal TEMPO_MENU_TRIG_SET[] = {
    {"pass", TEMPO_TrigFuncPass},
    {"forward", TEMPO_TrigFuncForward},
    {"backward", TEMPO_TrigFuncBackward},
    {"clear", TEMPO_TrigFuncClear},
    {"switch", TEMPO_TrigFuncSwitch},
    {NULL, NULL}
};
static const int TEMPO_MENU_TRIG_SET_LEN = sizeof(TEMPO_MENU_TRIG_SET) / sizeof(KeyVal);
const Table TEMPO_StaticTrigTable = {
    .len = sizeof(TEMPO_MENU_TRIG_SET) / sizeof(KeyVal),
    .kv = TEMPO_MENU_TRIG_SET
};
