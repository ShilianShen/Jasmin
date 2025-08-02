#include "menu.h"


const char* MENU_ROOT_NAME = "Root";
const char* MENU_EDGE_NAME = "Edge";


// MENU ================================================================================================================
Menu menu;


// GET & SET ===========================================================================================================
static Page* TEMPO_GetPageFromMenuPathId(const int pathId) {
    return menu.pages[menu.path[pathId]];
}
static Page* TEMPO_GetPageFromMenuPageId(const int pageId) {
    return menu.pages[pageId];
}


// INIT & DEINIT =======================================================================================================
void TEMPO_InitMenu() {
    // Req Condition
    if (tempoRenderer == NULL) {
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
    for (int i = 1; i < MENU_PAGE_VOLUME; i++) {
        if (menu.pages[i] != NULL) {
            printf("%s: tempo.pages[%d] shouldn't exist.\n", __func__, i);
            return;
        }
    }

    //
    menu = (Menu){0};
    menu.renderer = tempoRenderer;
    tempoRenderer = menu.renderer;
}
void TEMPO_DeinitMenu() {
    // Opt Condition
    if (menu.pageRoot != NULL) {
        TEMPO_DestroyPage(menu.pageRoot);
        menu.pageRoot = NULL;
    }
    if (menu.pageEdge != NULL) {
        TEMPO_DestroyPage(menu.pageEdge);
        menu.pageEdge = NULL;
    }
    for (int i = 1; i < MENU_PAGE_VOLUME; i++) {
        if (menu.pages[i] != NULL) {
            TEMPO_DestroyPage(menu.pages[i]);
            menu.pages[i] = NULL;
        }
    }
}


// LOAD & UNLOAD =======================================================================================================
static void TEMPO_LoadMenuThemeFont(const toml_datum_t tomlFontPath, const toml_datum_t tomlFontSize) {
    // Req Condition
    if (tomlFontPath.ok == false) {
        printf("%s: tomlFontPath not exists.\n", __func__); return;
    }
    if (tomlFontSize.ok == false) {
        printf("%s: tomlFontSize not exists.\n", __func__); return;
    }

    //
    TTF_Font* font = TTF_OpenFont(tomlFontPath.u.s, (float)tomlFontSize.u.d);
    if (font == NULL) {
        printf("%s: failed from %s.\n", __func__, tomlFontPath.u.s);
    }
    TEMPO_SetBasicFont(font);
}
static void TEMPO_LoadMenuTheme(const char* tomlPath) {
    // Req Condition
    toml_table_t* tomlMenuTheme = getToml(tomlPath); // malloc tomlMenuTheme
    if (tomlMenuTheme == NULL) {
        printf("%s: failed from \"%s\".\n", __func__, tomlPath);
        return;
    }

    //
    const toml_datum_t tomlFontPath = toml_string_in(tomlMenuTheme, "font_path");
    const toml_datum_t tomlFontSize = toml_double_in(tomlMenuTheme, "font_size");
    TEMPO_LoadMenuThemeFont(tomlFontPath, tomlFontSize);

    //
    toml_free(tomlMenuTheme); // free tomlMenuTheme
}
static void TEMPO_LoadMenuPages(const char* tomlPath) {
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
                TEMPO_DestroyPage(menu.pageRoot);
                menu.pageRoot = NULL;
            }
            menu.pageRoot = TEMPO_CreatePage(pageName, tomlPage);
        }
        else if (strcmp(pageName, MENU_EDGE_NAME) == 0) {
            if (menu.pageEdge != NULL) {
                TEMPO_DestroyPage(menu.pageEdge);
                menu.pageEdge = NULL;
            }
            menu.pageEdge = TEMPO_CreatePage(pageName, tomlPage);
        }
        else {
            if (menu.pages[pageId] != NULL) {
                TEMPO_DestroyPage(menu.pages[pageId]);
                menu.pages[pageId] = NULL;
            }
            menu.pages[pageId] = TEMPO_CreatePage(pageName, tomlPage);
            pageId++;
        }
    }

    //
    toml_free(tomlMenu); // end malloc
}
void TEMPO_LoadMenu() {
    const char* menuPagesPath = "../config/tempo/menu_pages.toml";
    const char* menuThemePath = "../config/tempo/menu_theme.toml";
    // Req Condition
    if (menu.renderer == NULL) {printf("%s: renderer not exists.\n", __func__); return;}

    //
    TEMPO_LoadMenuTheme(menuThemePath);
    TEMPO_LoadMenuPages(menuPagesPath);
}

static void TEMPO_UnloadMenuTheme() {
    TEMPO_UnloadBasic();
}
void TEMPO_UnloadMenu() {
    TEMPO_DestroyPage(menu.pageRoot);
    menu.pageRoot = NULL;
    TEMPO_DestroyPage(menu.pageEdge);
    menu.pageEdge = NULL;
    for (int i = 0; i < MENU_PAGE_VOLUME; i++) {
        if (menu.pages[i] != NULL) {
            TEMPO_DestroyPage(menu.pages[i]);
            menu.pages[i] = NULL;
        }
    }
    TEMPO_UnloadMenuTheme();
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