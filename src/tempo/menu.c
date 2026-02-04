#include "menu.h"


#include "../perph/perph.h"


// MENU ================================================================================================================
const char* MENU_ROOT_NAME = "Root";
const char* MENU_EDGE_NAME = "Edge";
bool trigAuthorize = false;
#define MENU_PATH_VOLUME 6
struct Menu {
    Table pageTable;
    Page* pageRoot;
    Page* pageEdge;
    Page* path[MENU_PATH_VOLUME];
    Page* pageNow;
};
typedef struct Menu Menu;
static Menu menu;


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





// INIT & EXIT =========================================================================================================
static bool TEMPO_InitMenu_RK(const cJSON* menu_json) {
    const char *key = NULL, *subkey = NULL;
    if (cJSON_ExistKey(menu_json, key = "pageTable")) {
        const cJSON* pageTable_json = cJSON_GetObjectItem(menu_json, key);
        REQ_CONDITION(pageTable_json != NULL, return false);
        REQ_CONDITION(BASIC_CreateTable(&menu.pageTable, TEMPO_CreatePage, pageTable_json), return false);
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
bool TEMPO_InitMenu() {
    memset(&menu, 0, sizeof(Menu));
    cJSON* menu_json = getJson(TEMPO_JSON);
    REQ_CONDITION(menu_json != NULL, return false);
    const bool rk = TEMPO_InitMenu_RK(menu_json);
    cJSON_Delete(menu_json);
    if (rk == false) {
        TEMPO_ExitMenu();
        return false;
    }
    for (int i = 0; i < menu.pageTable.len; i++) {
        printf("%s\n", menu.pageTable.kv[i].key);
    }
    return true;
}
void TEMPO_ExitMenu() {
    BASIC_DeleteTable(&menu.pageTable, TEMPO_DeletePage);
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

    trigAuthorize = false;
    for (int i = 0; menu.path[i+1] != NULL; i++) {
        TEMPO_RenewPage(menu.path[i]);
    }
    trigAuthorize = true;
    TEMPO_RenewPage(menu.pageNow);

    return true;
}


// DRAW ================================================================================================================
bool TEMPO_DrawMenu() {
    for (int i = 0; menu.path[i+1] != NULL; i++) {
        TEMPO_DrawPage(menu.path[i]);
    }
    TEMPO_DrawPage(menu.pageNow);
    return true;
}


// TRIG ================================================================================================================
static void TEMPO_TrigFuncPass(const char* para) {}
static void TEMPO_TrigFuncForward(const char* para) {
    REQ_CONDITION(para != NULL, return);
    const char* pageName = para;

    // getPage
    Page* page = NULL;
    for (int i = 0; i < menu.pageTable.len; i++) {
        if (menu.pageTable.kv[i].val == NULL) {continue;}
        if (strcmp(menu.pageTable.kv[i].key, pageName) == 0) {page = menu.pageTable.kv[i].val;}
    }
    if (page == NULL) {printf("%s: \"%s\" not exists.\n", __func__, (char*)pageName); return;}

    // forward
    for (int i = 0; i < MENU_PATH_VOLUME; i++) {
        if (menu.path[i] == 0) {
            menu.path[i] = page;
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

    if (page == NULL) return;

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
    {"swap", TEMPO_TrigFuncSwap},
};
const Table TEMPO_INNER_TRIG_FUNC_TABLE = {
    .len = len_of(TEMPO_INNER_TRIG_SET),
    .kv = TEMPO_INNER_TRIG_SET
};
