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
static bool TEMPO_LoadMenu_PageTable(const cJSON* pageTable_json) {
    if (pageTable_json == NULL) {
        printf("%s: tomlMenu == NULL.\n", __func__);
        return false;
    } // Req Condition

    menu.pageTable.len = cJSON_GetArraySize(pageTable_json);
    if (menu.pageTable.len == 0) {
        printf("%s: menu.lenPageSet == 0.\n", __func__);
        return false;
    } // Req Condition

    menu.pageTable.kv = calloc(menu.pageTable.len, sizeof(KeyVal));
    if (menu.pageTable.kv == NULL) {
        printf("%s: menu.pageSet == NULL.\n", __func__);
        return false;
    } // Req Condition

    for (int i = 0; i < menu.pageTable.len; i++) {
        const cJSON *item = cJSON_GetArrayItem(pageTable_json, i);
        if (item == NULL) {
            printf("%s: item == NULL.\n", __func__);
            return false;
        } // Req Condition

        const char* key = item->string;

        menu.pageTable.kv[i].key = strdup(key);
        if (menu.pageTable.kv[i].key == NULL) {
            printf("%s: menu.pageTable[i].key == NULL.\n", __func__);
            return false;
        } // Req Condition

        const cJSON* page_json = cJSON_GetObjectItem(pageTable_json, key);
        if (page_json == NULL) {
            printf("%s: tomlPage == NULL.\n", __func__);
            return false;
        } // Req Condition

        menu.pageTable.kv[i].val = TEMPO_CreatePage(page_json);
        if (menu.pageTable.kv[i].val == NULL) {
            printf("%s: menu.pageTable.kv[i].val == NULL.\n", __func__);
            return false;
        } // Req Condition

        if (strcmp(key, MENU_ROOT_NAME) == 0) {
            menu.pageRoot = menu.pageTable.kv[i].val;
        } // Opt Condition
        if (strcmp(key, MENU_EDGE_NAME) == 0) {
            menu.pageEdge = menu.pageTable.kv[i].val;
        } // Opt Condition
    }

    return true;
}
static bool TEMPO_LoadMenu_RK(const cJSON* menu_json) {

    const char *key = NULL, *subkey = NULL;
    if (cJSON_ExistKey(menu_json, key = "pageTable")) {
        const cJSON* pageSet_json = cJSON_GetObjectItem(menu_json, key);
        if (pageSet_json == NULL) {
            printf("%s: cJSON_GetObjectItem == NULL.\n", __func__);
            return false;
        } // Req Condition
        if (TEMPO_LoadMenu_PageTable(pageSet_json) == false) {
            printf("%s: TEMPO_LoadMenu_PageSet == false.\n", __func__);
            return false;
        } // Req Condition
    }
    if (cJSON_ExistKey(menu_json, key = "path")) {
        const cJSON* path_json = cJSON_GetObjectItem(menu_json, key);
        if (path_json == NULL) {
            printf("%s: cJSON_GetObjectItem == NULL.\n", __func__);
            return false;
        }
        if (cJSON_ExistKey(path_json, subkey = "pageNow")) {
            const char* pageNow_json = NULL;
            if (cJSON_LoadFromObj(path_json, subkey, JSM_STRING, &pageNow_json)) {
                menu.path[0] = TABLE_GetValByKey(menu.pageTable, pageNow_json);
            }
        }
    }

    return true;
}
bool TEMPO_LoadMenu() {
    memset(&menu, 0, sizeof(Menu));
#ifdef TEMPO_THEME_JSON
    cJSON* menu_json = getJson(TEMPO_MENU_JSON);
#else
    cJSON* menu_json = getJson(TEMPO_DEFAULT_MENU_JSON);
#endif
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
    DEBUG_SendMessageL("tempo.path: /%s", TABLE_GetKeyByVal((Table){menu.pageTable.len, menu.pageTable.kv}, menu.pageRoot));
    for (int i = 0; i < MENU_PATH_VOLUME; i++) {
        if (need_clear)
            menu.path[i] = NULL;
        else if (menu.path[i] == NULL)
            need_clear = true;
        else
            DEBUG_SendMessageL("/%s", TABLE_GetKeyByVal((Table){menu.pageTable.len, menu.pageTable.kv}, menu.path[i]));
    }
    if (need_clear == false) {
        DEBUG_SendMessageL("/%s", TABLE_GetKeyByVal((Table){menu.pageTable.len, menu.pageTable.kv}, menu.pageEdge));
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
void TEMPO_TrigFuncSwap(const char* para) {
    Page* page = TABLE_GetValByKey(menu.pageTable, para);
    if (page == NULL) {
        page = TABLE_GetValByKey(TEMPO_ExternPageTable, para);
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
static KeyVal TEMPO_MENU_TRIG_SET[] = {
    {"pass", TEMPO_TrigFuncPass},
    {"forward", TEMPO_TrigFuncForward},
    {"backward", TEMPO_TrigFuncBackward},
    {"clear", TEMPO_TrigFuncClear},
    {"switch", TEMPO_TrigFuncBool},
    {"swap", TEMPO_TrigFuncSwap},
    {"slid", TEMPO_TrigFuncSlid},
    {NULL, NULL}
};
const Table TEMPO_StaticTrigTable = {
    .len = sizeof(TEMPO_MENU_TRIG_SET) / sizeof(KeyVal),
    .kv = TEMPO_MENU_TRIG_SET
};
