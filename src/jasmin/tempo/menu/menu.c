#include "menu.h"


const char MENU_ROOT_NAME[] = "Root";
const char MENU_EDGE_NAME[] = "Edge";


Menu menu;


static Page* TEMPO_getPageFromMenuPathId(const int pathId) {
    return menu.pages[menu.path[pathId]];
}
static Page* TEMPO_GetPageFromMenuPageId(const int pageId) {
    return menu.pages[pageId];
}


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
    menu.pageRoot = malloc(sizeof(Page)); // malloc pageRoot
    menu.pageEdge = malloc(sizeof(Page)); // malloc pageEdge
    for (int i = 1; i < MENU_PAGE_VOLUME; i++) {
        if (menu.pages[i] == NULL) {
            menu.pages[i] = malloc(sizeof(Page)); // malloc pages
        }
    }
    tempoRenderer = menu.renderer;
}
void TEMPO_DeinitMenu() {
    // Opt Condition
    if (menu.pageRoot != NULL) {
        free(menu.pageRoot); // free pageRoot
        menu.pageRoot = NULL;
    }
    if (menu.pageEdge != NULL) {
        free(menu.pageEdge); // free pageEdge
        menu.pageEdge = NULL;
    }
    for (int i = 1; i < MENU_PAGE_VOLUME; i++) {
        if (menu.pages[i] != NULL) {
            free(menu.pages[i]); // free pages
            menu.pages[i] = NULL;
        }
    }
}

static void TEMPO_LoadMenuThemeFont(const toml_datum_t tomlFontPath, const toml_datum_t tomlFontSize) {
    // Req Condition
    if (menu.theme.font != NULL) {
        printf("%s: theme.font not free.\n", __func__); return;
    }
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
    menu.theme.font = font;
}
static void TEMPO_LoadMenuThemeColor(const toml_array_t* tomlColor) {
    // Req Condition
    if (tomlColor == NULL) {
        printf("%s: tomlColor not exists.\n", __func__);
        return;
    }

    //
    int color[4] = {};
    for (int i = 0; i < 4; i++) {
        const toml_datum_t c = toml_int_at(tomlColor, i);
        color[i] = c.ok ? (int)c.u.i : 255;
    }
    menu.theme.color.r = color[0];
    menu.theme.color.g = color[1];
    menu.theme.color.b = color[2];
    menu.theme.color.a = color[3];
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
    const toml_array_t* tomlColor = toml_array_in(tomlMenuTheme, "font_color");
    TEMPO_LoadMenuThemeFont(tomlFontPath, tomlFontSize);
    TEMPO_LoadMenuThemeColor(tomlColor);

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
            TEMPO_CreatePage(menu.pageRoot, pageName, tomlPage);
        }
        else if (strcmp(pageName, MENU_EDGE_NAME) == 0) {
            TEMPO_CreatePage(menu.pageEdge, pageName, tomlPage);
        }
        else {
            if (menu.pages[pageId] == NULL) {
                menu.pages[pageId] = malloc(sizeof(Page));
            }
            TEMPO_CreatePage(menu.pages[pageId], pageName, tomlPage);
            pageId++;
        }
    }

    //
    toml_free(tomlMenu); // end malloc
}
void TEMPO_LoadMenu(const char* menuPagesPath, const char* menuThemePath) {
    // Req Condition
    if (menu.renderer == NULL) {printf("%s: renderer not exists.\n", __func__); return;}

    //
    TEMPO_LoadMenuTheme(menuThemePath);
    TEMPO_LoadMenuPages(menuPagesPath);
}

static void TEMPO_UnloadMenuTheme() {
    if (menu.theme.font != NULL) {
        TTF_CloseFont(menu.theme.font);
        menu.theme.font = NULL;
    }
}
void TEMPO_UnloadMenu() {
    TEMPO_DestroyPage(menu.pageRoot);
    TEMPO_DestroyPage(menu.pageEdge);
    for (int i = 0; i < MENU_PAGE_VOLUME; i++) {
        if (menu.pages[i] != NULL) {
            TEMPO_DestroyPage(menu.pages[i]);
        }
    }
    TEMPO_UnloadMenuTheme();
}

static void TEMPO_RenewMenuPath() {
    //
    bool need_clear = false;
    DEBUG_SendMessageL("tempo.path: /%s", menu.pageRoot->name);
    for (int i = 0; i < MENU_PATH_VOLUME; i++) {
        if (need_clear) {
            menu.path[i] = 0;
        }
        else if (TEMPO_getPageFromMenuPathId(i) == NULL) {
            need_clear = true;
        }
        else {
            DEBUG_SendMessageL("/%s", TEMPO_getPageFromMenuPathId(i)->name);
        }
    }
    if (need_clear == false) {
        DEBUG_SendMessageL("/%s", menu.pageEdge->name);
    }
    DEBUG_SendMessageL("\n");
}
static void TEMPO_RenewMenuPageNow() {
    // if pageRoot
    if (TEMPO_getPageFromMenuPathId(0) == NULL) {
        menu.pageNow = menu.pageRoot;
        return;
    }
    // else if pageOther
    for (int i = 0; i + 1 < MENU_PATH_VOLUME; i++) {
        if (TEMPO_getPageFromMenuPathId(i) != NULL && TEMPO_getPageFromMenuPathId(i+1) == NULL) {
            menu.pageNow = TEMPO_getPageFromMenuPathId(i);
            return;
        }
    }
    // else pageEdge
    menu.pageNow = menu.pageEdge;
}
void TEMPO_RenewMenu(const SDL_FRect bck_rect) {
    static bool need_load = true;
    TEMPO_RenewMenuPath();
    TEMPO_RenewMenuPageNow();
    TEMPO_RenewPage(menu.pageNow);
    menu.bck_rect = bck_rect;
}

void TEMPO_DrawMenu() {
    TEMPO_DrawPage(menu.pageNow);
}