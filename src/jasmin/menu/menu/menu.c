#include "menu.h"


const char MENU_ROOT_NAME[] = "Root";
const char MENU_EDGE_NAME[] = "Edge";


Menu menu;


static Page* MENU_getPageFromPathId(const int pathId) {
    return menu.pages[menu.path[pathId]];
}
static Page* MENU_GetPageFromPageId(const int pageId) {
    return menu.pages[pageId];
}


void MENU_Init(SDL_Renderer* renderer) {
    // Req Condition
    if (renderer == NULL) {
        printf("%s: renderer not exists.\n", __func__);
        return;
    }
    if (menu.pageRoot != NULL) {
        printf("%s: menu.pageRoot shouldn't exist.\n", __func__);
        return;
    }
    if (menu.pageEdge != NULL) {
        printf("%s: menu.pageEdge shouldn't exist.\n", __func__);
        return;
    }
    for (int i = 1; i < MENU_PAGE_VOLUME; i++) {
        if (menu.pages[i] != NULL) {
            printf("%s: menu.pages[%d] shouldn't exist.\n", __func__, i);
            return;
        }
    }

    //
    menu = (Menu){0};
    menu.renderer = renderer;
    menu.pageRoot = malloc(sizeof(Page)); // malloc pageRoot
    menu.pageEdge = malloc(sizeof(Page)); // malloc pageEdge
    PAGE_Init(menu.pageRoot);
    PAGE_Init(menu.pageEdge);
    for (int i = 1; i < MENU_PAGE_VOLUME; i++) {
        if (menu.pages[i] == NULL) {
            menu.pages[i] = malloc(sizeof(Page)); // malloc pages
            PAGE_Init(menu.pages[i]);
        }
    }
    elem_renderer = menu.renderer;
}
void MENU_Deinit() {
    // Opt Condition
    if (menu.pageRoot != NULL) {
        PAGE_Deinit(menu.pageRoot);
        free(menu.pageRoot); // free pageRoot
        menu.pageRoot = NULL;
    }
    if (menu.pageEdge != NULL) {
        PAGE_Deinit(menu.pageEdge);
        free(menu.pageEdge); // free pageEdge
        menu.pageEdge = NULL;
    }
    for (int i = 1; i < MENU_PAGE_VOLUME; i++) {
        if (menu.pages[i] != NULL) {
            PAGE_Deinit(menu.pages[i]);
            free(menu.pages[i]); // free pages
            menu.pages[i] = NULL;
        }
    }
}
static toml_table_t* getToml(const char* tomlPath) {
    // Req Condition
    FILE* file = fopen(tomlPath, "r"); // malloc
    if (file == NULL) {printf("%s: failed to open \"%s\".\n", __func__, tomlPath); return NULL;}

    //
    toml_table_t* toml = toml_parse_file(file, NULL, 0); // malloc
    fclose(file); // end malloc

    // Req Condition
    if (toml == NULL) {printf("%s: failed to read \"%s\".\n", __func__, tomlPath); return NULL;}

    return toml;
}
static void MENU_LoadMenuThemeFont(const toml_datum_t tomlFontPath, const toml_datum_t tomlFontSize) {
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
    menu.theme.font = TTF_OpenFont(tomlFontPath.u.s, (float)tomlFontSize.u.d);
    if (menu.theme.font == NULL) {
        printf("%s: failed from %s.\n", __func__, tomlFontPath.u.s);
    }
    elem_font = menu.theme.font;
}
static void MENU_LoadMenuThemeColor(const toml_array_t* tomlColor) {
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
    elem_color = menu.theme.color;
}
static void MENU_LoadMenuTheme(const char* tomlPath) {
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
    MENU_LoadMenuThemeFont(tomlFontPath, tomlFontSize);
    MENU_LoadMenuThemeColor(tomlColor);

    //
    toml_free(tomlMenuTheme); // free tomlMenuTheme
}
static void MENU_LoadMenuPages(const char* tomlPath) {
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
            PAGE_Load(menu.pageRoot, pageName, tomlPage);
        }
        else if (strcmp(pageName, MENU_EDGE_NAME) == 0) {
            PAGE_Load(menu.pageEdge, pageName, tomlPage);
        }
        else {
            if (menu.pages[pageId] == NULL) {
                menu.pages[pageId] = malloc(sizeof(Page));
            }
            PAGE_Load(menu.pages[pageId], pageName, tomlPage);
            pageId++;
        }
    }

    //
    toml_free(tomlMenu); // end malloc
}
void MENU_Load(const char* menuPagesPath, const char* menuThemePath) {
    // Req Condition
    if (menu.renderer == NULL) {printf("%s: renderer not exists.\n", __func__); return;}

    //
    MENU_LoadMenuTheme(menuThemePath);
    MENU_LoadMenuPages(menuPagesPath);
    elem_color = menu.theme.color;
    elem_font = menu.theme.font;
}
static void MENU_UnloadMenuTheme() {
    if (menu.theme.font != NULL) {
        TTF_CloseFont(menu.theme.font);
        menu.theme.font = NULL;
    }
}
void MENU_Unload() {
    PAGE_Unload(menu.pageRoot);
    PAGE_Unload(menu.pageEdge);
    for (int i = 0; i < MENU_PAGE_VOLUME; i++) {
        if (menu.pages[i] != NULL) {
            PAGE_Unload(menu.pages[i]);
        }
    }
    MENU_UnloadMenuTheme();
}
static void MENU_RenewMenuPath() {
    bool need_clear = false;
    DEBUG_SendMessageL("menu.path: /%s", menu.pageRoot->name);
    for (int i = 0; i < MENU_PATH_VOLUME; i++) {
        if (need_clear) {
            menu.path[i] = 0;
        }
        else if (MENU_getPageFromPathId(i) == NULL) {
            need_clear = true;
        }
        else {
            DEBUG_SendMessageL("/%s", MENU_getPageFromPathId(i)->name);
        }
    }
    if (need_clear == false) {
        DEBUG_SendMessageL("/%s", menu.pageEdge->name);
    }
    DEBUG_SendMessageL("\n");
}
static void MENU_RenewMenuPageNow() {
    // if pageRoot
    if (MENU_getPageFromPathId(0) == NULL) {
        menu.pageNow = menu.pageRoot;
        return;
    }
    // else if pageOther
    for (int i = 0; i + 1 < MENU_PATH_VOLUME; i++) {
        if (MENU_getPageFromPathId(i) != NULL && MENU_getPageFromPathId(i+1) == NULL) {
            menu.pageNow = MENU_getPageFromPathId(i);
            return;
        }
    }
    // else pageEdge
    menu.pageNow = menu.pageEdge;
}
void MENU_Renew(SDL_FRect bck_rect) {
    static bool need_load = true;
    MENU_RenewMenuPath();
    MENU_RenewMenuPageNow();
    PAGE_Renew(menu.pageNow);
    // printf("%s\n", menu.pageNow->name);
    // printMenuPath();
    menu.bck_rect = bck_rect;
    elem_bck_rect = menu.bck_rect;
}
void MENU_Draw() {
    PAGE_Draw(menu.pageNow);
}