#include "menu.h"


const char* MENU_ROOT_NAME = "Root";
const char* MENU_EDGE_NAME = "Edge";
static const char* tomlPath = "../config/tempo/menu_pages.toml";


// MENU ================================================================================================================
Menu menu;


// GET & SET ===========================================================================================================
static Page* TEMPO_GetPageFromMenuPathId(const int pathId) {
    return menu.path[pathId];
}
static Page* TEMPO_GetPageFromMenuPageId(const int pageId) {
    return menu.pageSet[pageId];
}


// INIT & DEINIT =======================================================================================================
void TEMPO_InitMenu() {
    // Req Condition
    if (basic.renderer == NULL) {
        printf("%s: renderer not exists.\n", __func__);
        return;
    }
    if (menu.pageRoot != NULL) {
        printf("%s: tempo.pageRoot shouldn't exist.\n", __func__);
        return;
    }
    if (menu.pageEdge != NULL) {
        printf("%s: tempo.pageEdge shouldn't exist.\n", __func__);
        return;
    }
    for (int i = 1; i < LEN_PAGE_SET; i++) {
        if (menu.pageSet[i] != NULL) {
            printf("%s: tempo.pages[%d] shouldn't exist.\n", __func__, i);
            return;
        }
    }

    //
    menu = (Menu){0};
    menu.renderer = basic.renderer;
    basic.renderer = menu.renderer;
}
void TEMPO_DeinitMenu() {
    // Opt Condition
    if (menu.pageRoot != NULL) {
        menu.pageRoot = TEMPO_DeletePage(menu.pageRoot);
    }
    if (menu.pageEdge != NULL) {
        menu.pageEdge = TEMPO_DeletePage(menu.pageEdge);
    }
    for (int i = 1; i < LEN_PAGE_SET; i++) {
        if (menu.pageSet[i] != NULL) {
            menu.pageSet[i] = TEMPO_DeletePage(menu.pageSet[i]);
        }
    }
}


// LOAD & UNLOAD =======================================================================================================
static void TEMPO_LoadMenuPages() {
    // Req Condition
    toml_table_t* tomlMenu = getToml(tomlPath);
    if (tomlMenu == NULL) {printf("%s: failed from \"%s\".\n", __func__, tomlPath); return;}

    // getPageName
    int pageId = 1;
    for (int i = 0; toml_key_in(tomlMenu, i) != NULL; i++) {
        const char* pageName = toml_key_in(tomlMenu, i);
        const toml_table_t* tomlPage = toml_table_in(tomlMenu, pageName);
        if (tomlPage == NULL) {
            printf("%s: failed to get \"%s\" from tomlMenu[%p].\n", __func__, pageName, tomlMenu);
        }
        else if (strcmp(pageName, MENU_ROOT_NAME) == 0) {
            if (menu.pageRoot != NULL) {
                menu.pageRoot = TEMPO_DeletePage(menu.pageRoot);
            }
            menu.pageRoot = TEMPO_CreatePage(pageName, tomlPage);
        }
        else if (strcmp(pageName, MENU_EDGE_NAME) == 0) {
            if (menu.pageEdge != NULL) {
                menu.pageEdge = TEMPO_DeletePage(menu.pageEdge);
            }
            menu.pageEdge = TEMPO_CreatePage(pageName, tomlPage);
        }
        else {
            if (menu.pageSet[pageId] != NULL) {
                menu.pageSet[pageId] = TEMPO_DeletePage(menu.pageSet[pageId]);
            }
            menu.pageSet[pageId] = TEMPO_CreatePage(pageName, tomlPage);
            pageId++;
        }
    }

    //
    toml_free(tomlMenu); // end malloc
}
void TEMPO_LoadMenu() {
    // Req Condition
    if (menu.renderer == NULL) {printf("%s: renderer not exists.\n", __func__); return;}

    //
    TEMPO_LoadMenuPages();
}
void TEMPO_UnloadMenu() {
    menu.pageRoot = TEMPO_DeletePage(menu.pageRoot);
    menu.pageEdge = TEMPO_DeletePage(menu.pageEdge);
    for (int i = 0; i < LEN_PAGE_SET; i++) {
        if (menu.pageSet[i] != NULL) {
            menu.pageSet[i] = TEMPO_DeletePage(menu.pageSet[i]);
        }
    }
}


// RENEW ===============================================================================================================
static void TEMPO_RenewMenuPath() {
    //
    bool need_clear = false;
    DEBUG_SendMessageL("tempo.path: /%s", menu.pageRoot->name);
    for (int i = 0; i < MENU_PATH_VOLUME; i++) {
        if (need_clear) {
            menu.path[i] = 0;
        }
        else if (TEMPO_GetPageFromMenuPathId(i) == NULL) {
            need_clear = true;
        }
        else {
            DEBUG_SendMessageL("/%s", TEMPO_GetPageFromMenuPathId(i)->name);
        }
    }
    if (need_clear == false) {
        DEBUG_SendMessageL("/%s", menu.pageEdge->name);
    }
    DEBUG_SendMessageL("\n");
}
static void TEMPO_RenewMenuPageNow() {
    // if pageRoot
    if (TEMPO_GetPageFromMenuPathId(0) == NULL) {
        menu.pageNow = menu.pageRoot;
        return;
    }
    // else if pageOther
    for (int i = 0; i + 1 < MENU_PATH_VOLUME; i++) {
        if (TEMPO_GetPageFromMenuPathId(i) != NULL && TEMPO_GetPageFromMenuPathId(i+1) == NULL) {
            menu.pageNow = TEMPO_GetPageFromMenuPathId(i);
            return;
        }
    }
    // else pageEdge
    menu.pageNow = menu.pageEdge;
}
void TEMPO_RenewMenu() {
    static bool need_load = true;
    TEMPO_RenewMenuPath();
    TEMPO_RenewMenuPageNow();
    TEMPO_RenewPage(menu.pageNow);
}


// DRAW ================================================================================================================
void TEMPO_DrawMenu() {
    TEMPO_DrawPage(menu.pageNow);
}