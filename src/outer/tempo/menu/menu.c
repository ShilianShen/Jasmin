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
    toml_table_t* tomlMenu = getToml(tomlPath);
    if (tomlMenu == NULL) {
        printf("%s: tomlMenu == NULL.\n", __func__);
        return;
    } // Req Condition

    const toml_array_t* tomlPageSet = toml_array_in(tomlMenu, "pageSet");
    if (tomlPageSet == NULL) {
        printf("%s: tomlPageSet == NULL.\n", __func__);
        return;
    } // Req Condition

    menu.lenPageSet = toml_array_nelem(tomlPageSet);
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
        const toml_table_t* tomlPage = toml_table_at(tomlPageSet, i);
        if (tomlPage == NULL) {
            printf("%s: tomlPage == NULL.\n", __func__);
            return;
        } // Req Condition

        const toml_datum_t tomlName = toml_string_in(tomlPage, "name");
        if (tomlName.ok == false) {
            printf("%s: tomlName.ok == false.\n", __func__);
            return;
        } // Req Condition
        const char* name = tomlName.u.s;

        menu.pageSet[i] = TEMPO_CreatePage(tomlPage);
        if (strcmp(name, MENU_ROOT_NAME) == 0) {
            menu.pageRoot = menu.pageSet[i];
        }
        if (strcmp(name, MENU_EDGE_NAME) == 0) {
            menu.pageEdge = menu.pageSet[i];
        }
    }

    //
    toml_free(tomlMenu); // end malloc
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