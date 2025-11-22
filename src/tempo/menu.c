#include "menu.h"


const char* MENU_ROOT_NAME = "Root";
const char* MENU_EDGE_NAME = "Edge";
#define MENU_PATH_VOLUME 6
struct Menu {
    // malloc in INIT
    Table pageTable;
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
static bool TEMPO_LoadMenu_RK(const cJSON* menu_json) {
    const char *key = NULL, *subkey = NULL;
    if (cJSON_ExistKey(menu_json, key = "pageTable")) {
        const cJSON* pageTable_json = cJSON_GetObjectItem(menu_json, key);
        REQ_CONDITION(pageTable_json != NULL, return false);
        REQ_CONDITION(BASIC_CreateTable(&menu.pageTable, pageTable_json, TEMPO_CreatePage), return false);
        menu.pageRoot = BASIC_GetTableValByKey(menu.pageTable, MENU_ROOT_NAME);
        menu.pageEdge = BASIC_GetTableValByKey(menu.pageTable, MENU_EDGE_NAME);
    }
    if (cJSON_ExistKey(menu_json, key = "path")) {
        const cJSON* path_json = cJSON_GetObjectItem(menu_json, key);
        REQ_CONDITION(path_json != NULL, return false);
        if (cJSON_ExistKey(path_json, subkey = "pageNow")) {
            const char* pageNow_json = NULL;
            if (cJSON_LoadByKey(path_json, subkey, JSM_STRING, &pageNow_json)) {
                menu.path[0] = BASIC_GetTableValByKey(menu.pageTable, pageNow_json);
            }
        }
    }

    return true;
}
bool TEMPO_LoadMenu() {
    memset(&menu, 0, sizeof(Menu));
    cJSON* menu_json = getJson(TEMPO_DEFAULT_MENU_JSON);
    REQ_CONDITION(menu_json != NULL, return false);

    const bool rk = TEMPO_LoadMenu_RK(menu_json);
    cJSON_Delete(menu_json);

    if (rk == false) {
        TEMPO_UnloadMenu();
        return false;
    }
    return true;
}
void TEMPO_UnloadMenu() {
    for (int i = 0; i < menu.pageTable.len; i++) {
        free(menu.pageTable.kv[i].key);
        menu.pageTable.kv[i].key = NULL;
        menu.pageTable.kv[i].val = TEMPO_DeletePage(menu.pageTable.kv[i].val);
    }
}


// RENEW ===============================================================================================================
static void TEMPO_RenewMenuPath() {
    //
    bool need_clear = false;
    DEBUG_SendMessageL("tempo.path: /%s", BASIC_GetTableKeyByVal((Table){menu.pageTable.len, menu.pageTable.kv}, menu.pageRoot));
    for (int i = 0; i < MENU_PATH_VOLUME; i++) {
        if (need_clear)
            menu.path[i] = NULL;
        else if (menu.path[i] == NULL)
            need_clear = true;
        else
            DEBUG_SendMessageL("/%s", BASIC_GetTableKeyByVal((Table){menu.pageTable.len, menu.pageTable.kv}, menu.path[i]));
    }
    if (need_clear == false) {
        DEBUG_SendMessageL("/%s", BASIC_GetTableKeyByVal((Table){menu.pageTable.len, menu.pageTable.kv}, menu.pageEdge));
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
    for (int i = 0; i < menu.pageTable.len; i++) {
        if (menu.pageTable.kv[i].val == NULL) {continue;}
        if (strcmp(menu.pageTable.kv[i].key, pageName) == 0) {pageId = i;}
    }
    if (pageId == 0) {printf("%s: \"%s\" not exists.\n", __func__, (char*)pageName); return;}

    // forward
    for (int i = 0; i < MENU_PATH_VOLUME; i++) {
        if (menu.path[i] == 0) {
            menu.path[i] = menu.pageTable.kv[pageId].val;
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
static void TEMPO_TrigFuncSwap(const char* para) {
    Page* page = BASIC_GetTableValByKey(menu.pageTable, para);
    if (page == NULL) {
        page = BASIC_GetTableValByKey(TEMPO_ExternPageTable, para);
    }
    if (page == NULL) {
        return;
    }
    for (int i = 0; i < MENU_PATH_VOLUME; i++) {
        if (menu.path[i] == NULL) {
            menu.path[i-1] = page;
            return;
        }
    }
}
static KeyVal TEMPO_INNER_TRIG_SET[] = {
    {"pass", TEMPO_TrigFuncPass},
    {"forward", TEMPO_TrigFuncForward},
    {"backward", TEMPO_TrigFuncBackward},
    {"clear", TEMPO_TrigFuncClear},
    {"switch", TEMPO_TrigFuncBool},
    {"swap", TEMPO_TrigFuncSwap},
    {"slid", TEMPO_TrigFuncSlid},
};
const Table TEMPO_INNER_TRIG_TABLE = {
    .len = len_of(TEMPO_INNER_TRIG_SET),
    .kv = TEMPO_INNER_TRIG_SET
};
